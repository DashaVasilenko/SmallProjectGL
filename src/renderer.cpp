#include "renderer.h"
#include "light.h"
#include <iostream>

int Renderer::width;
int Renderer::height;

void Renderer::SetFrameBuffer(GLuint descriptor) {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, descriptor));  // привязываем или отвязываем свой буфер кадр 
}

void Renderer::SetActiveCamera(const Camera* camera) {
    this->camera = camera;
    this->projection = camera->GetProjectionMatrix();
}

void Renderer::Init() {
    GLCall(glViewport(0, 0, Renderer::width, Renderer::height));
    gbuffer.BufferInit(Renderer::width, Renderer::height);
    shadowbuffer.BufferInit(Renderer::width, Renderer::height);
    postprocessbuffer.BufferInit(Renderer::width, Renderer::height);
    current_view_buffer = postprocessbuffer.bloom.GetDescriptor();

// Говорим на какие слоты ждать текстуры
    toneMapPlusBrightness->Run();
    toneMapPlusBrightness->SetUniform("map", 0);

    textureView->Run();
    textureView->SetUniform("themap", 0);

    gaussProgram->Run();
    gaussProgram->SetUniform("brightMap", 0);
    gaussProgram->SetUniform("horizontal", true);

    bloomProgram->Run();
    bloomProgram->SetUniform("hdrMap", 0);
    bloomProgram->SetUniform("bloomBlur", 1);
}

void Renderer::ClearResult() {
    gbuffer.Bind();
    // TODO: Заменить на метод SetDrawBuffer
    GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT4));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::ShadowMapPass(entt::registry& registry) {
    auto meshes = registry.view<Mesh, Transform>(); // что рисуем

    GLCall(glEnable(GL_DEPTH_TEST)); // включаем тест глубины
    GLCall(glDepthMask(GL_TRUE)); // включаем запись в буфер глубины

    GLCall(glViewport(0, 0, shadowbuffer.GetSize(), shadowbuffer.GetSize())); // ставим размер карты теней
    shadowbuffer.Bind(); // биндим карту теней
    GLCall(glClear(GL_DEPTH_BUFFER_BIT)); // очищаем буфер глубины

    // TODO: Заменить на метод Framebuffer.SetDrawBuffers
    GLCall(glDrawBuffer(GL_NONE)); // не рисуем цвета (так как они нам не нужны, нужна только глубина)
   
    auto dir_lights = registry.view<DirectionalLight>();

    OrthoCamera orthoCamera;
    orthoCamera.SetProjection(-20.0f, 20.0f, -20.0f, 20.0f, 0.05f, 50.0f);
    const Camera* camera_save = camera;
    SetActiveCamera(&orthoCamera);

    GLCall(glCullFace(GL_FRONT)); // рендерим передние поверхности
    for (auto entity: dir_lights) {
        auto& light = dir_lights.get(entity);
        orthoCamera.SetPosition(glm::vec3(-20.0f, 20.0f, 0.0f));
        orthoCamera.SetFront(-light.GetDirection());
       
        lightMatrix = projection*orthoCamera.GetViewMatrix();

        for (auto entity: meshes) {
            auto& mesh = meshes.get<Mesh>(entity);
            auto& transform = meshes.get<Transform>(entity);
            mesh.DepthPass(lightMatrix, transform.GetModelMatrix());
        }
    }
    GLCall(glCullFace(GL_BACK)); // рендерим задние поверхности
    SetActiveCamera(camera_save); // возвращаем исходную камеру
    GLCall(glViewport(0, 0, Renderer::width, Renderer::height)); // устанавливаем размер окна для рендеринга
}

void Renderer::GeometryPass(entt::registry& registry) {
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    auto meshes = registry.view<Mesh, Transform>();

    gbuffer.Bind(); // биндим gbuffer
    // TODO: Строчки ниже убрать в метод фреймбуффера SetDrawBuffers
    unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3}; // выходы шейдера
    GLCall(glDrawBuffers(4, attachments)); // куда рисуем
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // чистим буфферы 
    
    // заполнение карт в gbufferе
    for (auto entity: meshes) {
        auto& mesh = meshes.get<Mesh>(entity);
        auto& transform = meshes.get<Transform>(entity);
        mesh.Draw(projection, viewMatrix, transform.GetModelMatrix());
    }

    GLCall(glDepthMask(GL_FALSE)); // запрещаем менять буффер глубины
}

void Renderer::BeginLightPass() {
    // TODO: Заменить на метод FrameBuffer.SetDrawBuffers();
    GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT4));

    // TODO: Заменить на метод Texture.Bind(GLenum)
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, gbuffer.position.GetDescriptor()));

    // TODO: Заменить на метод Texture.Bind(GLenum)
    GLCall(glActiveTexture(GL_TEXTURE1));
    GLCall(glBindTexture(GL_TEXTURE_2D, gbuffer.normal.GetDescriptor()));

    // TODO: Заменить на метод Texture.Bind(GLenum)
    GLCall(glActiveTexture(GL_TEXTURE2));
    GLCall(glBindTexture(GL_TEXTURE_2D, gbuffer.albedo.GetDescriptor()));

    // TODO: Заменить на метод Texture.Bind(GLenum)
    GLCall(glActiveTexture(GL_TEXTURE3));
    GLCall(glBindTexture(GL_TEXTURE_2D, gbuffer.metallRoughAO.GetDescriptor()));

    GLCall(glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF));
    GLCall(glDisable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendEquation(GL_FUNC_ADD));
    GLCall(glBlendFunc(GL_ONE, GL_ONE));
}

void Renderer::EndLightPass() {
    GLCall(glCullFace(GL_BACK));
    GLCall(glDisable(GL_BLEND));
}

void Renderer::BeginStencilPass() {
    GLCall(glDrawBuffer(GL_NONE));
    GLCall(glEnable(GL_DEPTH_TEST));

    GLCall(glDisable(GL_CULL_FACE));
    
    GLCall(glClear(GL_STENCIL_BUFFER_BIT));
    GLCall(glStencilFunc(GL_ALWAYS, 0, 0));
    GLCall(glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP));
    GLCall(glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP));
}

void Renderer::LightPass(entt::registry& registry) {
    glm::mat4 viewMatrix = camera->GetViewMatrix();

    GLCall(glEnable(GL_STENCIL_TEST));
    auto lights = registry.view<PointLight>();
    for (auto entity: lights) {
        auto& pl = lights.get(entity);
        // Прежде чем рисовать свет нужно отбросить все что не попадает в сферу
        BeginStencilPass();
        pl.StencilPass(projection, viewMatrix);

        BeginLightPass();
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        pl.SetInnerUniforms();
        pl.Draw(projection, viewMatrix);
        EndLightPass();   
    }

    auto spotlights = registry.view<SpotLight>();
    for (auto entity: spotlights) {
        auto& sl = spotlights.get(entity);

        BeginStencilPass();
        sl.StencilPass(projection, viewMatrix);

        BeginLightPass();
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        sl.SetInnerUniforms();
        sl.Draw(projection, viewMatrix);
        EndLightPass();
    }

    GLCall(glDisable(GL_STENCIL_TEST);)
    GLCall(glDisable(GL_CULL_FACE));
    auto dlights = registry.view<DirectionalLight>();

    for (auto entity: dlights) {
        auto& dl = dlights.get(entity);

        BeginLightPass();
        GLCall(glActiveTexture(GL_TEXTURE4));
        GLCall(glBindTexture(GL_TEXTURE_2D, shadowbuffer.depthMap.GetDescriptor()));
        dl.SetInnerUniforms();
        dl.Draw(viewMatrix, lightMatrix);
        GLCall(glDisable(GL_BLEND));
    }    
}

void Renderer::PostProcess() {
    // Биндим резалт текстуру

    // TODO: Заменить на метод Texture.Bind(GLenum)
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, gbuffer.result.GetDescriptor()));

    // Биндим фреймбуффер для постпроцессинга и указываем 2 выхода
    postprocessbuffer.Bind();
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    // TODO: Заменить на метод класса framebuffer
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    GLCall(glDrawBuffers(2, attachments));    

    toneMapPlusBrightness->Run();
    quad->Draw(); 

    // Заполнили hdrMap и brightMap
    bool horizontal = true;
    bool first_iteration = true;
    int cnt = 5;
 
    for (int i = 0; i < cnt; i++) {
         
        GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT2));
        if (first_iteration) {
            GLCall(glActiveTexture(GL_TEXTURE0));
            GLCall(glBindTexture(GL_TEXTURE_2D, postprocessbuffer.brightMap.GetDescriptor())); 
            first_iteration = false;
        }
        else {
            GLCall(glActiveTexture(GL_TEXTURE0));
            GLCall(glBindTexture(GL_TEXTURE_2D, postprocessbuffer.verticalGauss.GetDescriptor())); 
        }

        gaussProgram->Run();
        gaussProgram->SetUniform("horizontal", horizontal);
        quad->Draw();
        horizontal = !horizontal;

        GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT3));
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, postprocessbuffer.horizontalGauss.GetDescriptor())); 
        gaussProgram->Run();
        gaussProgram->SetUniform("horizontal", horizontal);
        quad->Draw();
        horizontal = !horizontal;
     }


    GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT4)); // указали, в какой канал будем рисовать

    GLCall(glActiveTexture(GL_TEXTURE0)); // прифигачили одну текстуру на 0 слот
    GLCall(glBindTexture(GL_TEXTURE_2D, postprocessbuffer.hdrMap.GetDescriptor()));

    GLCall(glActiveTexture(GL_TEXTURE1)); // прифигачили вторую текстуру на 1 слот
    GLCall(glBindTexture(GL_TEXTURE_2D, postprocessbuffer.verticalGauss.GetDescriptor()));

    bloomProgram->Run();
    quad->Draw();
    



    postprocessbuffer.Unbind();

    // Выводим результат на экран
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, current_view_buffer));

    textureView->Run();
    quad->Draw();
}


void Renderer::SkyBoxRender(entt::registry& registry) {
    glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
    
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthMask(GL_FALSE));
    GLCall(glDepthFunc(GL_LEQUAL));
    
    auto skyboxes = registry.view<SkyBox>();
    for (auto entity: skyboxes) {
        auto& skybox = skyboxes.get(entity);
        skybox.Draw(projection, view);
    }

    GLCall(glDepthFunc(GL_LESS));
    GLCall(glDepthMask(GL_TRUE));
    GLCall(glDisable(GL_DEPTH_TEST)); 
    
}


/* 
void Renderer::BeginForwardRendering() {
    // копируем буффер глубины в окошко!
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.descriptor);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // буфер глубины по-умолчанию
    glBlitFramebuffer(
        0, 0, Renderer::width, Renderer::height, 0, 0, Renderer::width, Renderer::height, GL_DEPTH_BUFFER_BIT, GL_NEAREST
    );
}
*/


void Renderer::DebugLightDraw(entt::registry& registry) {
    glm::mat4 viewMatrix = camera->GetViewMatrix();

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthMask(GL_TRUE));

    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    auto point_lights = registry.view<PointLight>();
    for (auto entity: point_lights) {
        auto& pl = point_lights.get(entity);    
        pl.DebugDraw(projection, viewMatrix);
    }

    auto spotlights = registry.view<SpotLight>();
    for (auto entity: spotlights) {
        auto& sl = spotlights.get(entity);
        sl.DebugDraw(projection, viewMatrix);
    }
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    GLCall(glDepthMask(GL_FALSE));
    GLCall(glDisable(GL_DEPTH_TEST));
}

void Renderer::Update(entt::registry& registry) {
    ClearResult();
    ShadowMapPass(registry);
    GeometryPass(registry);
    LightPass(registry);
    // здесь рисуем прозрачные штуки и разные вещи, которые не вписываются в deferred rendering
    SkyBoxRender(registry);
    if (light_debug) {
        DebugLightDraw(registry);
    }
    
    PostProcess();
   
}

bool Renderer::WireFrame(const std::vector<std::string>& arguments) {
    if (arguments.size() == 1) {
        bool flag = std::stoi(arguments[0]);
        if (flag) {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        }
        else {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        }
        return true;
    }
    return false;
} 

bool Renderer::DebugLight(const std::vector<std::string>& arguments) {
    if (arguments.size() == 1) {
        bool flag = std::stoi(arguments[0]);
        if (flag) {
            light_debug = true;
        }
        else {
            light_debug = false;
        }
        return true;
    }
    return false;
} 

bool Renderer::ViewBuffer(const std::vector<std::string>& arguments) {
    if (arguments.size() == 1) {
        if (arguments[0] == "normal") {
            current_view_buffer = gbuffer.normal.GetDescriptor();
            return true;
        }
        else if (arguments[0] == "position") {
            current_view_buffer = gbuffer.position.GetDescriptor();
            return true;
        }
        else if (arguments[0] == "albedo") {
            current_view_buffer = gbuffer.albedo.GetDescriptor();
            return true;
        }
        else if (arguments[0] == "mrao") {
            current_view_buffer = gbuffer.metallRoughAO.GetDescriptor();
            return true;
        }
        else if (arguments[0] == "nohdr") {
            current_view_buffer = gbuffer.result.GetDescriptor();
            return true;
        }
        else if (arguments[0] == "hdr") {
            current_view_buffer = postprocessbuffer.hdrMap.GetDescriptor();
            return true;
        }
        else if (arguments[0] == "shadowmap") {
            current_view_buffer = shadowbuffer.depthMap.GetDescriptor();
            return true;
        }
        else if (arguments[0] == "brightmap") {
            current_view_buffer = postprocessbuffer.brightMap.GetDescriptor();
            return true;
        }
        else if (arguments[0] == "gauss") {
            current_view_buffer = postprocessbuffer.verticalGauss.GetDescriptor();
            //current_view_buffer = postprocessbuffer.horizontalGauss.GetDescriptor();
            return true;
        }
        else if (arguments[0] == "bloom") {
            current_view_buffer = postprocessbuffer.bloom.GetDescriptor();
            return true;
        }
        return false;
    }
    return false;
}
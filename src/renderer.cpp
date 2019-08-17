#include "renderer.h"
#include "light.h"
#include <iostream>

int Renderer::width;
int Renderer::height;

void Renderer::SetFrameBuffer(GLuint descriptor) {
    glBindFramebuffer(GL_FRAMEBUFFER, descriptor);  // привязываем или отвязываем свой буфер кадр 
}

void Renderer::SetActiveCamera(const Camera* camera) {
    this->camera = camera;
    this->projection = camera->GetProjectionMatrix();
}


void Renderer::Init() {
    glViewport(0, 0, Renderer::width, Renderer::height);
    gbuffer.BufferInit(Renderer::width, Renderer::height);
    shadowbuffer.BufferInit(Renderer::width, Renderer::height);
    postprocessbuffer.BufferInit(Renderer::width, Renderer::height);
    current_view_buffer = postprocessbuffer.hdrMap;


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
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ShadowMapPass(entt::registry& registry) {
    // что рисуем
    auto meshes = registry.view<Mesh, Transform>();

    // включаем тест глубины
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE); 

    // ставим размер карты
    glViewport(0, 0, shadowbuffer.GetSize(), shadowbuffer.GetSize());
    // начинаем рендер в карту теней
    shadowbuffer.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);

    // TODO: Заменить на метод Framebuffer.SetDrawBuffers
    glDrawBuffer(GL_NONE);
   
    auto dir_lights = registry.view<DirectionalLight>();

    OrthoCamera orthoCamera;
    orthoCamera.SetAspect(width/height);
    orthoCamera.SetProjection(-30.0f*orthoCamera.GetAspect(), 30.0f*orthoCamera.GetAspect(), -30.0f, 30.0f, 0.05f, 50.0f);
    const Camera* camera_save = camera;
    SetActiveCamera(&orthoCamera);

    glCullFace(GL_FRONT);
    for (auto entity: dir_lights) {
        auto& light = dir_lights.get(entity);
        orthoCamera.SetPosition(glm::vec3(-10.0f, 10.0f, 10.0f));
        orthoCamera.SetFront(-light.GetDirection());
       
        lightMatrix = projection*orthoCamera.GetViewMatrix();

        for (auto entity: meshes) {
            auto& mesh = meshes.get<Mesh>(entity);
            auto& transform = meshes.get<Transform>(entity);
            mesh.DepthPass(lightMatrix, transform.GetModelMatrix());
        }
    }
    glCullFace(GL_BACK);
    SetActiveCamera(camera_save);
    glViewport(0, 0, Renderer::width, Renderer::height);
    // Конец заполнения карты теней 
}

void Renderer::GeometryPass(entt::registry& registry) {
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    auto meshes = registry.view<Mesh, Transform>();

    // Биндим gbuffer указываем 4 выхода шейдерам
    gbuffer.Bind();
    // TODO: Строчки ниже убрать в метод фреймбуффера SetDrawBuffers
    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // заполнение карт в gbufferе
    for (auto entity: meshes) {
        auto& mesh = meshes.get<Mesh>(entity);
        auto& transform = meshes.get<Transform>(entity);
        mesh.Draw(projection, viewMatrix, transform.GetModelMatrix());
    }

    // Запрещаем менять буффер глубины
    glDepthMask(GL_FALSE);    
}

void Renderer::BeginLightPass() {
    // TODO: Заменить на метод FrameBuffer.SetDrawBuffers();
    glDrawBuffer(GL_COLOR_ATTACHMENT4);

    // TODO: Заменить на метод Texture.Bind(GLenum)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.position);

    // TODO: Заменить на метод Texture.Bind(GLenum)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbuffer.normal);

    // TODO: Заменить на метод Texture.Bind(GLenum)
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gbuffer.albedo);

    // TODO: Заменить на метод Texture.Bind(GLenum)
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gbuffer.metallRoughAO);

    glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
}

void Renderer::EndLightPass() {
    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
}

void Renderer::BeginStencilPass() {
    glDrawBuffer(GL_NONE);
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);
    
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 0, 0);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
}

void Renderer::LightPass(entt::registry& registry) {
    glm::mat4 viewMatrix = camera->GetViewMatrix();

    glEnable(GL_STENCIL_TEST);
    auto lights = registry.view<PointLight>();
    for (auto entity: lights) {
        auto& pl = lights.get(entity);
        // Прежде чем рисовать свет нужно отбросить все что не попадает в сферу
        BeginStencilPass();
        pl.StencilPass(projection, viewMatrix);

        BeginLightPass();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
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
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        sl.SetInnerUniforms();
        sl.Draw(projection, viewMatrix);
        EndLightPass();
    }

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    auto dlights = registry.view<DirectionalLight>();

    for (auto entity: dlights) {
        auto& dl = dlights.get(entity);

        BeginLightPass();
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, shadowbuffer.depthMap);
        dl.SetInnerUniforms();
        dl.Draw(viewMatrix, lightMatrix);
        glDisable(GL_BLEND);
    }    
}

void Renderer::PostProcess() {
      // Биндим резалт текстуру

    // TODO: Заменить на метод Texture.Bind(GLenum)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.result);

    // Биндим фреймбуффер для постпроцессинга и указываем 2 выхода
    postprocessbuffer.Bind();
    glClear(GL_COLOR_BUFFER_BIT);
    // TODO: Заменить на метод класса framebuffer
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);    

    toneMapPlusBrightness->Run();
    quad->Draw(); 
    // Заполнили hdrMap и brightMap


//////////////////////////////////////////// 
  /*  bool horizontal = true;

    glDrawBuffer(GL_COLOR_ATTACHMENT2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, postprocessbuffer.brightMap);

    gaussProgram->Run();
    gaussProgram->SetUniform("horizontal", horizontal);
    quad->Draw();
    horizontal = !horizontal;

    glDrawBuffer(GL_COLOR_ATTACHMENT3);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, postprocessbuffer.horizontalGauss); 
        
    gaussProgram->Run();
    gaussProgram->SetUniform("horizontal", horizontal);
    quad->Draw();
    horizontal = !horizontal;*/
 

///////////////////////////////////////////

 
    bool horizontal = true;
    bool first_iteration = true;
    int cnt = 5;
 
    for (unsigned int i = 0; i < cnt; i++) {
         
        glDrawBuffer(GL_COLOR_ATTACHMENT2);
        if (first_iteration) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, postprocessbuffer.brightMap); 
            first_iteration = false;
        }
        else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, postprocessbuffer.verticalGauss); 
        }

        gaussProgram->Run();
        gaussProgram->SetUniform("horizontal", horizontal);
        quad->Draw();
        horizontal = !horizontal;

        glDrawBuffer(GL_COLOR_ATTACHMENT3);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, postprocessbuffer.horizontalGauss); 
        gaussProgram->Run();
        gaussProgram->SetUniform("horizontal", horizontal);
        quad->Draw();
        horizontal = !horizontal;
     }


    glDrawBuffer(GL_COLOR_ATTACHMENT4); // указали, в какой канал будем рисовать

    glActiveTexture(GL_TEXTURE0); // прифигачили одну текстуру на 0 слот
    glBindTexture(GL_TEXTURE_2D, postprocessbuffer.hdrMap);

    glActiveTexture(GL_TEXTURE1); // прифигачили вторую текстуру на 1 слот
    glBindTexture(GL_TEXTURE_2D, postprocessbuffer.verticalGauss);

    bloomProgram->Run();
    quad->Draw();
    



    postprocessbuffer.Unbind();

    // Выводим результат на экран
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, current_view_buffer);

    textureView->Run();
    quad->Draw();
}

void Renderer::BeginForwardRendering() {
    // копируем буффер глубины в окошко!
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.descriptor);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // буфер глубины по-умолчанию
    glBlitFramebuffer(
        0, 0, Renderer::width, Renderer::height, 0, 0, Renderer::width, Renderer::height, GL_DEPTH_BUFFER_BIT, GL_NEAREST
    );
}

void Renderer::DebugLightDraw(entt::registry& registry) {
    glm::mat4 viewMatrix = camera->GetViewMatrix();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::Update(entt::registry& registry) {
    ClearResult();
    ShadowMapPass(registry);
    GeometryPass(registry);
    LightPass(registry);
    PostProcess();
    
    if (light_debug) {
        BeginForwardRendering();
        DebugLightDraw(registry);
    }
   
}

bool Renderer::WireFrame(const std::vector<std::string>& arguments) {
    if (arguments.size() == 1) {
        bool flag = std::stoi(arguments[0]);
        if (flag) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
            current_view_buffer = gbuffer.normal;
            return true;
        }
        else if (arguments[0] == "position") {
            current_view_buffer = gbuffer.position;
            return true;
        }
        else if (arguments[0] == "albedo") {
            current_view_buffer = gbuffer.albedo;
            return true;
        }
        else if (arguments[0] == "mrao") {
            current_view_buffer = gbuffer.metallRoughAO;
            return true;
        }
        else if (arguments[0] == "nohdr") {
            current_view_buffer = gbuffer.result;
            return true;
        }
        else if (arguments[0] == "hdr") {
            current_view_buffer = postprocessbuffer.hdrMap;
            return true;
        }
        else if (arguments[0] == "shadowmap") {
            current_view_buffer = shadowbuffer.depthMap;
            return true;
        }
        else if (arguments[0] == "brightmap") {
            current_view_buffer = postprocessbuffer.brightMap;
            return true;
        }
        else if (arguments[0] == "gauss") {
            current_view_buffer = postprocessbuffer.verticalGauss;
            //current_view_buffer = postprocessbuffer.horizontalGauss;
            return true;
        }
        else if (arguments[0] == "bloom") {
            current_view_buffer = postprocessbuffer.bloom;
            return true;
        }
        return false;
    }
    return false;
}
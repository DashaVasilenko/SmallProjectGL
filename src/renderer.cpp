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
	glViewport(0, 0, Renderer::width, Renderer::height); // позиция нижнего левого угла окна и размер области в окне, в котором рисуем   
    gbuffer.BufferInit(Renderer::width, Renderer::height);
    shadowbuffer.BufferInit(Renderer::width, Renderer::height);
    current_view_buffer = gbuffer.GetAlbedoDescriptor();
}

void Renderer::GeometryPass(entt::registry& registry) {
    // Бинд буффера геометрии, теперь рисуем всё в него  



    glm::mat4 viewMatrix = camera->GetViewMatrix();
    auto meshes = registry.view<Mesh, Transform>();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE); 


    // Заполнение карты теней
    glViewport(0, 0, shadowbuffer.GetSize(), shadowbuffer.GetSize()); // позиция нижнего левого угла окна и размер области в окне, в котором рисуем   
    shadowbuffer.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_NONE); // Никакого рендеринга
   
    auto dir_lights = registry.view<DirectionalLight>();
    OrthoCamera cam;
    cam.SetAspect(width/height);
    cam.SetProjection(-30.0f*cam.GetAspect(), 30.0f*cam.GetAspect(), -30.0f, 30.0f, 0.05f, 50.0f);

    const Camera* camera_save = camera;

    glCullFace(GL_FRONT);
    for (auto entity: dir_lights) {
        auto& light = dir_lights.get(entity);
        cam.SetPosition(glm::vec3(-10.0f, 10.0f, 10.0f));
        cam.SetFront(-light.GetDirection());
        SetActiveCamera(&cam);
        lightMatrix = projection*cam.GetViewMatrix();

        for (auto entity: meshes) {
            auto& mesh = meshes.get<Mesh>(entity);
            auto& transform = meshes.get<Transform>(entity);
            mesh.DepthPass(projection, cam.GetViewMatrix(), transform.GetModelMatrix());
        }
    }
    glCullFace(GL_BACK);
    SetActiveCamera(camera_save);
    glViewport(0, 0, Renderer::width, Renderer::height); // позиция нижнего левого угла окна и размер области в окне, в котором рисуем   



    gbuffer.GeometryPassBind();
    // включаем тест глубины чтобы яйки друг за другом рисовались без багов
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    //

    
    for (auto entity: meshes) {
        auto& mesh = meshes.get<Mesh>(entity);
        auto& transform = meshes.get<Transform>(entity);
        mesh.Draw(projection, viewMatrix, transform.GetModelMatrix());
    }

     
    
    

    glDepthMask(GL_FALSE);

    // Конец заполнения карты теней
}

void Renderer::BeginLightPass() {
    gbuffer.LightPassBind();
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
    gbuffer.StencilPassBind();
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
        shadowbuffer.Bind();
        shadowbuffer.LightPassBind();
        gbuffer.Bind();
        dl.SetInnerUniforms();
        dl.Draw(viewMatrix, lightMatrix); // set uniform ViewToLightSpace
        glDisable(GL_BLEND);

        //EndLightPass();
    }
    
    // Here do directional light!
}

void Renderer::FinalPass() {

    gbuffer.FinalPassBind();
    glBlitFramebuffer(0, 0, Renderer::width, Renderer::height,
                      0, 0, Renderer::width, Renderer::height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    gbuffer.Unbind();

    /*shadowbuffer.Bind();
    shadowbuffer.BindDepth();
    shadowbuffer.Unbind();


    ShaderProgram* quadProgram = Engine::programManager.Get("data/shaders/quad.json");
    Geometry* quad = Engine::geometryManager.Get("data/quad.obj");

    quadProgram->Run();
    quadProgram->SetUniform("map", 0);
    quad->Draw(); 

    
    shadowbuffer.Unbind();*/
}

void Renderer::Update(entt::registry& registry) {
    gbuffer.StartFrame();
    // Рисуем позиции нормали альбедо мрао в color_attachment 0 1 2 3
    GeometryPass(registry);
    // Рисуем финальный результат в color_attachment4
    LightPass(registry);
    FinalPass();
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


bool Renderer::ViewBuffer(const std::vector<std::string>& arguments) {
    if (arguments.size() == 1) {
        if (arguments[0] == "normal") {
            current_view_buffer = gbuffer.GetNormalDescriptor();
            return true;
        }
        else if (arguments[0] == "position") {
            current_view_buffer = gbuffer.GetPositionDescriptor();
            return true;
        }
        else if (arguments[0] == "albedo") {
            current_view_buffer = gbuffer.GetAlbedoDescriptor();
            return true;
        }
        else if (arguments[0] == "mrao") {
            current_view_buffer = gbuffer.GetMetallRoughAODescriptor();
            return true;
        }
        return false;
    }
    return false;
}

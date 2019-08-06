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
    current_view_buffer = gbuffer.GetAlbedoDescriptor();
}

void Renderer::GeometryPass(entt::registry& registry) {
    // Бинд буффера геометрии, теперь рисуем всё в него    
    gbuffer.GeometryPassBind();
    glDepthMask(GL_TRUE); // только проход геометрии обновляет буффер глубины
    // включаем тест глубины чтобы яйки друг за другом рисовались без багов
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glEnable(GL_DEPTH_TEST);

    // Проход геометрии
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    auto meshes = registry.view<Mesh, Transform>();
    for (auto entity: meshes) {
        auto& mesh = meshes.get<Mesh>(entity);
        auto& transform = meshes.get<Transform>(entity);
        mesh.Draw(projection, viewMatrix, transform.GetModelMatrix());
    }

    glDepthMask(GL_FALSE); // запрещаем менять Depth buffer

    // Копируем буффер глубины из gBuffer -> screen
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.GetDescriptor()); // откуда с джибуффера
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // куда в экран
    //glBlitFramebuffer(0, 0, Renderer::width, Renderer::height, 0, 0, Renderer::width, Renderer::height, GL_DEPTH_BUFFER_BIT, GL_NEAREST); // копируем
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

    auto dlights = registry.view<DirectionalLight>();

    for (auto entity: dlights) {
        auto& dl = dlights.get(entity);

        BeginLightPass();
        dl.SetInnerUniforms();
        dl.Draw(viewMatrix);
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

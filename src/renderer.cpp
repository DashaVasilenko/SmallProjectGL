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
   
    //fbo.BufferInit(width, height); // создаем буфер кадра
    gbuffer.BufferInit(Renderer::width, Renderer::height);

    current_view_buffer = gbuffer.GetAlbedoDescriptor();
}

void Renderer::Update(entt::registry& registry) {
    // здесь нужно сказать что мы рисуем в текстуру  
    //fbo.Bind(); 
    glDisable(GL_BLEND);
    gbuffer.Bind();
    
    glEnable(GL_DEPTH_TEST); // тест глубины

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очищаем буферы
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // буфер цвета очищаем синим цветом

    // Проход геометрии
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    auto meshes = registry.view<Mesh, Transform>();
    for (auto entity: meshes) {
        auto& mesh = meshes.get<Mesh>(entity);
        auto& transform = meshes.get<Transform>(entity);
        mesh.Draw(projection, viewMatrix, transform.GetModelMatrix());
    }

    gbuffer.Unbind();

    
    glDisable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT); // очищаем буферы
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // буфер цвета очищаем синим цветом
    
    glEnable(GL_BLEND);
    //glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    // glDepthMask(0);
    // glCullFace(GL_FRONT);   //to render only backfaces
    // glDepthFunc(GL_GEQUAL);
    // Проход света
    // Включаем смешивание

    
    auto lights = registry.view<PointLight>();
    for (auto entity: lights) {
        auto& pl = lights.get(entity);
        pl.SetInnerUniforms();

        /* Сделать gBuffer статическим в Renderer и убрать бинд текстур в SetInnerUniforms */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbuffer.GetPositionDescriptor());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gbuffer.GetNormalDescriptor());

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gbuffer.GetAlbedoDescriptor());

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gbuffer.GetMetallRoughAODescriptor());

        pl.Draw(projection, viewMatrix);
    }
}

bool Renderer::WireFrame(const std::vector<std::string>& arguments) {
    if (arguments.size() == 1) {
        bool flag = std::stoi(arguments[0]);
        if (flag) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
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

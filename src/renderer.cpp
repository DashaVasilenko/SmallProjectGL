#include "renderer.h"
#include <iostream>

void Renderer::SetWidth(int width) {
    this->width = width;
}

void Renderer::SetHeight(int height) {
    this->height = height;
}

void Renderer::SetFrameBuffer(GLuint descriptor) {
    glBindFramebuffer(GL_FRAMEBUFFER, descriptor);  // привязываем или отвязываем свой буфер кадр 
}

void Renderer::SetActiveCamera(const Camera* camera) {
    this->camera = camera;
    this->projection = camera->GetProjectionMatrix();
}
 
void Renderer::Init(const ShaderProgram* qprogram, const Geometry* qgeometry) {
	glViewport(0, 0, width, height); // позиция нижнего левого угла окна и размер области в окне, в котором рисуем
   
    fbo.BufferInit(width, height); // создаем буфер кадра
    quad_program = qprogram;
    quad_geometry = qgeometry;
}

void Renderer::Update(entt::registry& registry) {
    // здесь нужно сказать что мы рисуем в текстуру  
    fbo.Bind();
    glEnable(GL_DEPTH_TEST); // тест глубины
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очищаем буферы
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // буфер цвета очищаем синим цветом

    

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    auto view = registry.view<Mesh, Transform>();
    for (auto entity: view) {
        auto& mesh = view.get<Mesh>(entity);
        auto& transform = view.get<Transform>(entity);
        mesh.Draw(projection, viewMatrix, transform.GetModelMatrix());
    }
    fbo.Unbind();
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT); // очищаем буферы
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // буфер цвета очищаем синим цветом

    quad_program->Run();
    
    glBindTexture(GL_TEXTURE_2D, fbo.GetTexDescriptor());
    quad_geometry->Draw();
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

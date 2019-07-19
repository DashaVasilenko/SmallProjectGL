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

void Renderer::AddMesh(const Mesh* mesh) {
    meshes.push_back(mesh);
}
 
void Renderer::Init() {
	glViewport(0, 0, width, height); // позиция нижнего левого угла окна и размер области в окне, в котором рисуем
    glEnable(GL_DEPTH_TEST); // тест глубины
    fbo.BufferInit(width, height); // создаем буфер кадра
}

void Renderer::Update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очищаем буферы
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // буфер цвета очищаем синим цветом

    glm::mat4 view = camera->GetViewMatrix();
    for (auto& mesh: meshes) {
        mesh->Draw(projection, view);
    }

}
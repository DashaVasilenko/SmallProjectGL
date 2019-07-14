#include "renderer.h"
#include <iostream>

void Renderer::SetWidth(int width) {
    this->width = width;
}

void Renderer::SetHeight(int height) {
    this->height = height;
}

void Renderer::SetActiveCamera(const Camera* camera) {
    this->camera = camera;
    this->projection = camera->GetProjectionMatrix();
}

void Renderer::Init() {
	glViewport(0, 0, width, height); // позиция нижнего левого угла окна и размер области в окне, в котором рисуем
    glEnable(GL_DEPTH_TEST); // тест глубины
}

void Renderer::Update(entt::registry& registry) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очищаем буферы
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // буфер цвета очищаем синим цветом

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    auto view = registry.view<Mesh>();
    for (auto entity: view) {
        auto &mesh = view.get(entity);
        mesh.Draw(projection, viewMatrix);
    }

}
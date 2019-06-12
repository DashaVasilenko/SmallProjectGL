#include "renderer.h"

void Renderer::SetWidth(int width) {
    this->width = width;
}

void Renderer::SetHeight(int height) {
    this->height = height;
}

void Renderer::Init() {
	glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::Update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}
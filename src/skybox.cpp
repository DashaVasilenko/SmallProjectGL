#include "skybox.h"

SkyBox::SkyBox() {
    skybox = Engine::geometryManager.Get("data/cube.obj");
    skyboxProgram = Engine::programManager.Get("data/shaders/skybox.json"); 
}

void SkyBox::Draw(const glm::mat4& projection, const glm::mat4& view) {
    skyboxProgram->Run();
    //skyboxProgram->SetUniform("MVP", projection*view*glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 50.0f, 50.0f)));
    skyboxProgram->SetUniform("MVP", projection*glm::mat4(glm::mat3(view)));
    skybox->Draw();
}

void SkyBox::Init(const std::array<std::string, 6>& fileNames) {
    cubemap.Bind();
    cubemap.Init(fileNames);
}
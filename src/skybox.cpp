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
    GLCall(glGenTextures(1, &descriptor));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, descriptor));

    int width, height, nrChannels;
    for(unsigned int i = 0; i < fileNames.size(); i++)
    {
        unsigned char *data = stbi_load(fileNames[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << fileNames[i] << std::endl;
            stbi_image_free(data);
        }
     }

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));  
}

SkyBox::~SkyBox() {
    GLCall(glDeleteTextures(1, &descriptor));
} 
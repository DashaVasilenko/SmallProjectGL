#ifndef __SKYBOX__
#define __SKYBOX__

#include <iostream>

#include "geometry.h"
#include "shaderProgram.h"
#include "resourceManager.h"
#include "engine.h"


class SkyBox {
public:
    SkyBox();
    void Draw(const glm::mat4& projection, const glm::mat4& view);
    void Init(const std::array<std::string, 6>& fileNames);
    ~SkyBox();

private:
    unsigned int descriptor;

    Geometry* skybox;
    ShaderProgram* skyboxProgram;
     
};

#endif /* End of __SKYBOX__ */
#ifndef __SKYBOX__
#define __SKYBOX__

#include <iostream>

#include "geometry.h"
#include "shaderProgram.h"
#include "resourceManager.h"
#include "engine.h"
#include "errors.h"
#include "texture.h"


class SkyBox {
public:
    SkyBox();
    void Draw(const glm::mat4& projection, const glm::mat4& view);
    void Init(const std::array<std::string, 6>& fileNames);

private:
    CubeMap cubemap;
    Geometry* skybox;
    ShaderProgram* skyboxProgram;
};

#endif /* End of __SKYBOX__ */
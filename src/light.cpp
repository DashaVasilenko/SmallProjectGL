#include "light.h"
#include <iostream>

DirectLight::DirectLight(const ShaderProgram* shaderProgram, 
                         const glm::vec3& kA, const glm::vec3& kD, const glm::vec3& kS, 
                         const glm::vec3& dir) 
{
    this->shaderProgram = shaderProgram;
    this->ambientColor = kA;
    this->diffuseColor = kD;
    this->specularColor = kS;
    this->direction = dir;
}

void DirectLight::SetInnerUniforms() const {
    shaderProgram->Run();
    shaderProgram->SetUniform("direct_light.ambientColor", ambientColor);
    shaderProgram->SetUniform("direct_light.diffuseColor", diffuseColor);
    shaderProgram->SetUniform("direct_light.specularColor", specularColor);
    shaderProgram->SetUniform("direct_light.direction", direction);
}

//-------------------------------------------------------------------------------------------------
//
//
PointLight::PointLight(const ShaderProgram* shaderProgram, 
                       const glm::vec3& kA, const glm::vec3& kD, const glm::vec3& kS, 
                       const glm::vec3& pos) 
{
    this->shaderProgram = shaderProgram;
    this->ambientColor = kA;
    this->diffuseColor = kD;
    this->specularColor = kS;
    this->position = pos;

}

void PointLight::SetInnerUniforms() const {
    shaderProgram->Run();
    shaderProgram->SetUniform("point_light.ambientLightColor", ambientColor);
    shaderProgram->SetUniform("point_light.diffuseLightColor", diffuseColor);
    shaderProgram->SetUniform("point_light.specularLightColor", specularColor);
    shaderProgram->SetUniform("point_light.position", position);
 
    shaderProgram->SetUniform("point_light.Kc", Kc);
    shaderProgram->SetUniform("point_light.Kl", Kl);
    shaderProgram->SetUniform("point_light.Kq", Kq);
}
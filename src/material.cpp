#include "material.h"

void Material::SetProjectionMatrix(const glm::mat4& projection) {
    shaderProgram->SetUniform("Projection", projection); // перегоняет в координаты clip space;
}

void Material::SetViewMatrix(const glm::mat4& view) {
    shaderProgram->SetUniform("View", view); // перегоняет координаты в пространство камеры
}

void Material::SetModelMatrix(const glm::mat4& model) {
    shaderProgram->SetUniform("Model", model); // масштабирование повороты и смещение
    glm::mat3 model3x3 = model;
    shaderProgram->SetUniform("NormalMatrix", glm::transpose(glm::inverse(model3x3)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
PhongMaterial::PhongMaterial(const ShaderProgram* shaderProgram, const glm::vec3& kA, const glm::vec3& kD, const glm::vec3& kS, float s) {
    this->shaderProgram = shaderProgram;
    this->ambientColor = kA;
    this->diffuseColor = kD;
    this->specularColor = kS;
    this->shininess = s;
}

void PhongMaterial::SetInnerUniforms() {
    shaderProgram->SetUniform("material.ambientColor", ambientColor);
    shaderProgram->SetUniform("material.diffuseColor", diffuseColor);
    shaderProgram->SetUniform("material.specularColor", specularColor);
    shaderProgram->SetUniform("material.shininess", shininess);
}
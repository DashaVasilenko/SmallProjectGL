#include "material.h"

void Material::SetProjectionMatrix(const glm::mat4& projection) const {
    shaderProgram->SetUniform("Projection", projection); // перегоняет в координаты clip space;
}

void Material::SetViewMatrix(const glm::mat4& view) const {
    shaderProgram->SetUniform("View", view); // перегоняет координаты в пространство камеры
}

void Material::SetModelMatrix(const glm::mat4& model) const {
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

void PhongMaterial::SetInnerUniforms() const {
    shaderProgram->SetUniform("material.ambientColor", ambientColor);
    shaderProgram->SetUniform("material.diffuseColor", diffuseColor);
    shaderProgram->SetUniform("material.specularColor", specularColor);
    shaderProgram->SetUniform("material.shininess", shininess);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
PhongTextureMaterial::PhongTextureMaterial(
                        const ShaderProgram* shaderProgram, 
                        const Texture* ambientColor,
                        const Texture* diffuseColor, 
                        const Texture* specularColor,
                        float s) 
{
    this->shaderProgram = shaderProgram;
    this->ambientMap = ambientColor;
    this->diffuseMap = diffuseColor;
    this->specularMap = specularColor;
    this->shininess = s;
}

void PhongTextureMaterial::SetInnerUniforms() const {
    glActiveTexture(GL_TEXTURE0);
    ambientMap->Bind();
    shaderProgram->SetUniform("ambientMap", 0);

    glActiveTexture(GL_TEXTURE1);
    diffuseMap->Bind();
    shaderProgram->SetUniform("diffuseMap", 1);

    glActiveTexture(GL_TEXTURE2);
    specularMap->Bind();
    shaderProgram->SetUniform("specularMap", 2);

    shaderProgram->SetUniform("material.shininess", shininess);
}
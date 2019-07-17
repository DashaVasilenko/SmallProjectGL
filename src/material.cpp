#include "material.h"

void Material::SetProjectionMatrix(const glm::mat4& projection) const {
    shaderProgram->SetUniform("Projection", projection); // перегоняет в координаты clip space;
}

void Material::SetViewMatrix(const glm::mat4& view) const {
    shaderProgram->SetUniform("View", view); // перегоняет координаты в пространство камеры
}

void Material::SetModelMatrix(const glm::mat4& model) const {
    shaderProgram->SetUniform("Model", model); // масштабирование повороты и смещение
}

void Material::SetNormalMatrix(const glm::mat3& normalMatrix) const {
    shaderProgram->SetUniform("NormalMatrix", normalMatrix);
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
                        const Texture* normals,
                        float s) 
{
    this->shaderProgram = shaderProgram;
    this->ambientMap = ambientColor;
    this->diffuseMap = diffuseColor;
    this->specularMap = specularColor;
    this->normalMap = normals;
    this->shininess = s;

    shaderProgram->Run();
    shaderProgram->SetUniform("ambientMap", 0);
    shaderProgram->SetUniform("diffuseMap", 1);
    shaderProgram->SetUniform("specularMap", 2);
    shaderProgram->SetUniform("normalMap", 3);
}

void PhongTextureMaterial::SetInnerUniforms() const {
    glActiveTexture(GL_TEXTURE0);
    ambientMap->Bind();

    glActiveTexture(GL_TEXTURE1);
    diffuseMap->Bind();
   
    glActiveTexture(GL_TEXTURE2);
    specularMap->Bind();

    glActiveTexture(GL_TEXTURE3);
    normalMap->Bind();
   
    shaderProgram->SetUniform("material.shininess", shininess);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
PbrMaterial::PbrMaterial(
                        const ShaderProgram* shaderProgram, 
                        const Texture* albedo,
                        const Texture* normals, 
                        const Texture* metallic,
                        const Texture* roughness,
                        const Texture* ao) 
{
    this->shaderProgram = shaderProgram;
    this->albedoMap = albedo;
    this->normalMap = normals;
    this->metallicMap = metallic;
    this->roughnessMap = roughness;
    this->aoMap = ao;

    shaderProgram->Run();
    shaderProgram->SetUniform("albedoMap", 0);
    shaderProgram->SetUniform("normalMap", 1);
    shaderProgram->SetUniform("metallicMap", 2);
    shaderProgram->SetUniform("roughnessMap", 3);
    shaderProgram->SetUniform("aoMap", 4);
}

void PbrMaterial::SetInnerUniforms() const {
    glActiveTexture(GL_TEXTURE0);
    albedoMap->Bind();

    glActiveTexture(GL_TEXTURE1);
    normalMap->Bind();
   
    glActiveTexture(GL_TEXTURE2);
    metallicMap->Bind();

    glActiveTexture(GL_TEXTURE3);
    roughnessMap->Bind();

    glActiveTexture(GL_TEXTURE4);
    aoMap->Bind();
}
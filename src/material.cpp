#include "material.h"
#include "engine.h"
#include <iostream>

void Material::SetProjectionMatrix(const glm::mat4& projection) {
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
PhongMaterial::PhongMaterial(ShaderProgram* shaderProgram, const glm::vec3& kA, const glm::vec3& kD, const glm::vec3& kS, float s) {
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
PhongTextureMaterial::PhongTextureMaterial(
                        ShaderProgram* shaderProgram, 
                        const Texture2D* ambientColor,
                        const Texture2D* diffuseColor, 
                        const Texture2D* specularColor,
                        const Texture2D* normals,
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

void PhongTextureMaterial::SetInnerUniforms() {
    ambientMap->CreateAttachment(GL_TEXTURE0);
    diffuseMap->CreateAttachment(GL_TEXTURE1);   
    specularMap->CreateAttachment(GL_TEXTURE2);
    normalMap->CreateAttachment(GL_TEXTURE3);
   
    shaderProgram->SetUniform("material.shininess", shininess);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
PbrMaterial::PbrMaterial(
                        ShaderProgram* shaderProgram, 
                        const Texture2D* albedo,
                        const Texture2D* normals, 
                        const Texture2D* metallic,
                        const Texture2D* roughness,
                        const Texture2D* ao) 
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

void PbrMaterial::SetInnerUniforms() {
    albedoMap->CreateAttachment(GL_TEXTURE0);
    normalMap->CreateAttachment(GL_TEXTURE1);
    metallicMap->CreateAttachment(GL_TEXTURE2);
    roughnessMap->CreateAttachment(GL_TEXTURE3);
    aoMap->CreateAttachment(GL_TEXTURE4);
}

/////////////////////////////////////////////////////////////////////////////////////////
void MaterialPBR::SetAlbedo(const std::string& fileName) {
    albedoMap = Engine::textureManager.Get(fileName);
    albedo_texture = true;
}

void MaterialPBR::SetAlbedo(const glm::vec3& color) {
    albedo_color = color;
    albedo_texture = false;
}

void MaterialPBR::SetRoughness(const std::string& fileName) {
    roughnessMap = Engine::textureManager.Get(fileName);
    roughness_texture = true;
}

void MaterialPBR::SetRoughness(float r) {
    roughness = r;
    roughness_texture = false;
}

void MaterialPBR::SetMetallic(const std::string& fileName) {
    metallicMap = Engine::textureManager.Get(fileName);
    metallic_texture = true;
}

void MaterialPBR::SetMetallic(float m) {
    metallic = m;
    metallic_texture = false;
}

void MaterialPBR::SetNormalMap(const std::string& fileName, NormalFlag flag) {
    switch(flag) {
        case NORMAL_MAP:
            normalMap = Engine::textureManager.Get(fileName);
            normal_texture = true;
            break;
        case DISPLACEMENT_MAP:
            displacementMap = Engine::textureManager.Get(fileName);
            displacement_texture = true;
    }
}

void MaterialPBR::SetAO(const std::string& fileName) {
    aoMap = Engine::textureManager.Get(fileName);
    ao_texture = true;
}

void MaterialPBR::SetScale(float scale) {
    texture_scale = scale;
}

void MaterialPBR::Init() {
    std::string program_name = "pbr_";

    if (albedo_texture) {
        program_name +="a";
    }
    if (normal_texture) {
        program_name +="n";
    }
    if (roughness_texture) {
        program_name +="r";
    }
    if (metallic_texture) {
        program_name +="m";
    }
    if (ao_texture) {
        program_name +="ao";
    }

    if (!albedo_texture && !normal_texture && !roughness_texture && !metallic_texture && !ao_texture) {
        program_name ="pbr";
    }

    program_name += ".json";

    shaderProgram = Engine::programManager.Get("data/shaders/"+program_name);

    shaderProgram->Run();
    if (albedo_texture) {
        shaderProgram->SetUniform("albedoMap", 0);
    }
   
    if (normal_texture) {
        shaderProgram->SetUniform("normalMap", 1);
    }

    if (metallic_texture) {
         shaderProgram->SetUniform("metallicMap", 2);
    }
       
    if (roughness_texture) {
        shaderProgram->SetUniform("roughnessMap", 3);
    }
       
    if (ao_texture) {
        shaderProgram->SetUniform("aoMap", 4);
    }
}

void MaterialPBR::SetInnerUniforms() {
	//shaderProgram->Run();
    if (albedo_texture) {
        albedoMap->CreateAttachment(GL_TEXTURE0);
    }
    else {
        shaderProgram->SetUniform("albedo", albedo_color);
    }
        
    if (normal_texture) {
        normalMap->CreateAttachment(GL_TEXTURE1);
    }

    if (metallic_texture) {
        metallicMap->CreateAttachment(GL_TEXTURE2);
    }
    else {
        shaderProgram->SetUniform("metallic", metallic);
    }
       
    if (roughness_texture) {
        roughnessMap->CreateAttachment(GL_TEXTURE3);
    }
    else {
        shaderProgram->SetUniform("roughness", roughness);
    }
       
    if (ao_texture) {
        aoMap->CreateAttachment(GL_TEXTURE4);
    }

	if (albedo_texture || normal_texture || roughness_texture || metallic_texture || ao_texture) {
		shaderProgram->SetUniform("texture_scale", texture_scale);
	}
}
#ifndef __MATERIAL__
#define __MATERIAL__

#include "shaderProgram.h"
#include "texture.h"

class Material {
public:
    inline void Bind() const { shaderProgram->Run(); }
    virtual void SetInnerUniforms() = 0;
    void SetProjectionMatrix(const glm::mat4&);
    void SetViewMatrix(const glm::mat4&) const;
    void SetModelMatrix(const glm::mat4&) const;
    void SetNormalMatrix(const glm::mat3&) const;

protected:
    ShaderProgram* shaderProgram;
};

class PhongMaterial: public Material{
public:
    PhongMaterial(ShaderProgram* shaderProgram, const glm::vec3&, const glm::vec3&, const glm::vec3&, float);
    void SetInnerUniforms() override;

private:
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
};

class PhongTextureMaterial: public Material{
public:
    PhongTextureMaterial(ShaderProgram*, const Texture2D*, const Texture2D* , const Texture2D*, const Texture2D*, float);
    void SetInnerUniforms() override;

private:
    const Texture2D* ambientMap;
    const Texture2D* diffuseMap;
    const Texture2D* specularMap;
    const Texture2D* normalMap;
    float shininess;
};

class PbrMaterial: public Material{
public:
    PbrMaterial(ShaderProgram*, const Texture2D*, const Texture2D* , const Texture2D*, const Texture2D*, const Texture2D*);
    void SetInnerUniforms() override;

private:
    const Texture2D* albedoMap;
    const Texture2D* normalMap;
    const Texture2D* metallicMap;
    const Texture2D* roughnessMap;
    const Texture2D* aoMap;
};

enum NormalFlag { NORMAL_MAP, DISPLACEMENT_MAP };

class MaterialPBR: public Material {
public:
    MaterialPBR(){}
    void SetAlbedo(const std::string& fileName);
    void SetAlbedo(const glm::vec3& color);
    void SetNormalMap(const std::string& fileName, NormalFlag flag);
    void SetRoughness(const std::string& fileName);
    void SetRoughness(float roughness);
    void SetMetallic(const std::string& fileName);
    void SetMetallic(float metallic);
    void SetAO(const std::string& fileName);
    void SetScale(float scale);

    void Init();
    void SetInnerUniforms() override;
    
private:
    const Texture2D* albedoMap;
    const Texture2D* metallicMap;
    const Texture2D* roughnessMap;
    const Texture2D* normalMap;
    const Texture2D* displacementMap;
    const Texture2D* aoMap;

    glm::vec3 albedo_color = glm::vec3(1.0f, 0.0f, 0.0f);

    float roughness = 0.0f;
    float metallic = 0.0f;

    float texture_scale = 1.0f;

    bool albedo_texture = false;
    bool normal_texture = false;
    bool displacement_texture = false;
    bool roughness_texture = false;
    bool metallic_texture = false;
    bool ao_texture = false;
};

#endif /* End of Material */
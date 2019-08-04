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
    PhongTextureMaterial(ShaderProgram*, const Texture*, const Texture* , const Texture*, const Texture*, float);
    void SetInnerUniforms() override;
private:
    const Texture* ambientMap;
    const Texture* diffuseMap;
    const Texture* specularMap;
    const Texture* normalMap;
    float shininess;
};

class PbrMaterial: public Material{
public:
    PbrMaterial(ShaderProgram*, const Texture*, const Texture* , const Texture*, const Texture*, const Texture*);
    void SetInnerUniforms() override;
private:
    const Texture* albedoMap;
    const Texture* normalMap;
    const Texture* metallicMap;
    const Texture* roughnessMap;
    const Texture* aoMap;
};


#endif /* End of Material */
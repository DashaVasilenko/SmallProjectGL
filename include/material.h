#ifndef __MATERIAL__
#define __MATERIAL__

#include "shaderProgram.h"

class Material {
public:
    inline void Bind() const { shaderProgram->Run(); }
    virtual void SetInnerUniforms() const = 0;
    void SetProjectionMatrix(const glm::mat4&) const;
    void SetViewMatrix(const glm::mat4&) const;
    void SetModelMatrix(const glm::mat4&) const;
protected:
    const ShaderProgram* shaderProgram;
};

class PhongMaterial: public Material{
public:
    PhongMaterial(const ShaderProgram* shaderProgram, const glm::vec3&, const glm::vec3&, const glm::vec3&, float);
    void SetInnerUniforms() const override;
private:
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
};


#endif /* End of Material */
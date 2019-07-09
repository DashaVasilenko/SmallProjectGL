#ifndef __MATERIAL__
#define __MATERIAL__

#include "shaderProgram.h"

class Material {
public:
    inline void Bind() { shaderProgram->Run(); }
    virtual void SetInnerUniforms() = 0;
    void SetProjectionMatrix(const glm::mat4&);
    void SetViewMatrix(const glm::mat4&);
    void SetModelMatrix(const glm::mat4&);
protected:
    const ShaderProgram* shaderProgram;
};

class PhongMaterial: public Material{
public:
    PhongMaterial(const ShaderProgram* shaderProgram, const glm::vec3&, const glm::vec3&, const glm::vec3&, float);
    void SetInnerUniforms() override;
private:
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
};


#endif /* End of Material */
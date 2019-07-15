#ifndef __LIGHT__
#define __LIGHT__

#include "shaderProgram.h"

class Light {
public:
    virtual void SetInnerUniforms() const = 0;
    //virtual ~Light() { }
protected:
    const ShaderProgram* shaderProgram;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    // float intensity;
};

class DirectLight : public Light{
public:
    DirectLight(const ShaderProgram* shaderProgram, const glm::vec3&, const glm::vec3&, const glm::vec3&, const glm::vec3&);
    void SetInnerUniforms() const override;
private:
    glm::vec3 direction;
};

class PointLight : public Light{
public:
    PointLight(const ShaderProgram* shaderProgram, const glm::vec3&, const glm::vec3&, const glm::vec3&, const glm::vec3&);
    void setAttenuation(float k1, float k2, float k3) { assert(k1 >= 0 && k2 >= 0 && k3 >=0); Kc = k1; Kl = k2; Kq =k3; } // затенение
    void SetInnerUniforms() const override;
private:
    glm::vec3 position;
    float Kc = 1.0f; // константный коэффициент
    float Kl = 0.0f; // линейный коэффициент
    float Kq = 0.0f; // квадратичный коэффициент
};

#endif /* End of Light */
#ifndef __LIGHT__
#define __LIGHT__

#include "shaderProgram.h"
#include "geometry.h"
#include "camera.h"

// Базовый класс для любого света кроме Directional
class Light {
public:
    virtual void SetInnerUniforms() = 0;
    virtual ~Light(){} 

protected:
    Geometry* geometry;  
    ShaderProgram* shaderProgram;
   
    glm::vec3 color;
    glm::vec3 position_WS;  // World space position
    float intensity; 
};

class DirectionalLight : public Light{
public:
    DirectionalLight(const glm::vec3&, const glm::vec3&);
    void SetInnerUniforms() override;
    void Draw(const glm::mat4& view, const glm::mat4& light);
    glm::vec3 GetDirection() { return direction; }
    virtual ~DirectionalLight(){}

private:
    glm::vec3 direction;
};

class PointLight : public Light{
public:
    PointLight(const glm::vec3&, const glm::vec3&, float intensity = 10.0f);
    void SetAttenuation(float k1, float k2, float k3); 
    void CalculateRadius();
    void SetInnerUniforms() override;
    void Draw(const glm::mat4&, const glm::mat4&);
    void DebugDraw(const glm::mat4&, const glm::mat4&);
    void StencilPass(const glm::mat4&, const glm::mat4&);
    virtual ~PointLight(){}

private:
    ShaderProgram* stencilProgram;
    ShaderProgram* debugProgram;

    glm::mat4 model = glm::mat4(1.0f);
    /* Attenuation is given by the quadratic function*/
    float Kc = 1.0f; // const coeff
    float Kl = 0.7f; // linear coeff
    float Kq = 1.8f; // quadratic coeff
    float radius;
};

class SpotLight : public Light{
public:
    SpotLight(const glm::vec3& color, const glm::vec3& pos, const glm::vec3& dir, float angle, float intensity = 1.0f, float exponent = 15.0f);
    void SetAttenuation(float k1) { assert(k1 >= 0); exponent = k1;  }
    void SetInnerUniforms() override;
    void Draw(const glm::mat4& projection, const glm::mat4& view);
    void DebugDraw(const glm::mat4& projection, const glm::mat4& view);
    void StencilPass(const glm::mat4& projection, const glm::mat4& view);
    virtual ~SpotLight() {}

private:
    ShaderProgram* stencilProgram;
    ShaderProgram* debugProgram;
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 direction;
    float cutoff; 
    float exponent;
};

#endif /* End of Light */
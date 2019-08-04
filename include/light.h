#ifndef __LIGHT__
#define __LIGHT__

#include "shaderProgram.h"
#include "geometry.h"


/* Базовый класс для любого света */
class Light {
public:
    virtual void SetInnerUniforms() = 0;
    virtual void Draw(const glm::mat4& projection, const glm::mat4& view) = 0;
protected:
    /* Теперь у света есть своя геометрия */
    Geometry* geometry;
    ShaderProgram* shaderProgram;
    glm::vec3 color;
    /* Позиции в WorldSpace*/
    glm::vec3 position_WS;
    /* интенсивность */
    float intensity;
};

/*class DirectionalLight : public Light{
public:
    DirectionalLight(const glm::vec3&, const glm::vec3&);
    void SetInnerUniforms() const override;
private:
    glm::vec3 direction;
};*/


class PointLight : public Light{
public:
    PointLight(const glm::vec3&, const glm::vec3&, float intensity = 100.0f);
    void SetAttenuation(float k1, float k2, float k3); 
    void CalculateRadius();
    void SetInnerUniforms() override;
    void Draw(const glm::mat4&, const glm::mat4&) override;
private:

    /* по дефолту свет затухает с квадратичной скоростью */
    glm::mat4 model = glm::mat4(1.0f);

    float Kc = 0.0f; // константный коэффициент
    float Kl = 0.0f; // линейный коэффициент
    float Kq = 1.0f; // квадратичный коэффициент
    float radius;
};




/*class SpotLight : public Light{
public:
    SpotLight(const glm::vec3&, const glm::vec3&, const glm::vec3&, float);
    void setAttenuation(float k1) { assert(k1 >= 0); exponent = k1;  } // затухание света
    void SetInnerUniforms() const override;
private:
    glm::vec3 position;
    glm::vec3 direction;
    float cutoff; // угол отсечки (от 0 до 90 градусов)
    float exponent; // экспонента углового ослабления силы света
};*/

#endif /* End of Light */
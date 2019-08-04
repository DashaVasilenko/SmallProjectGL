#include "light.h"
#include "engine.h"
#include "renderer.h"

/*DirectionalLight::DirectionalLight(const glm::vec3& color, const glm::vec3& dir) 
{
    //this->shaderProgram = Engine::programManager.Get(...);
    this->color = color;
    this->direction = dir;
}

void DirectionalLight::SetInnerUniforms() const {
    shaderProgram->Run();
    shaderProgram->SetUniform("direct_light.color", color);
    shaderProgram->SetUniform("direct_light.direction", direction);
}*/

//-------------------------------------------------------------------------------------------------
//
//
PointLight::PointLight(const glm::vec3& color, 
                       const glm::vec3& pos,
                       float intensity) 
{
    /*  
        РесурсМенеджеры перенесены в Engine и они теперь статические,
        поэтому не обязательно передавать текстуры, шейдерпрограммы и геометрию,
        через параметры функций! 
    */

    this->shaderProgram = Engine::programManager.Get("data/shaders/point_light.json");
    this->stencilProgram = Engine::programManager.Get("data/shaders/stencil.json");
    this->color = color*intensity;
    this->intensity = intensity;
    this->position_WS = pos;
    this->geometry = Engine::geometryManager.Get("data/light_sphere.obj");

    CalculateRadius();
    /* Сетим юниформы которые не меняются каждый кадр, сетим слоты текстур */
    this->shaderProgram->Run();
    
    this->shaderProgram->SetUniform("ScreenWidthHeight", glm::vec2(Renderer::GetWidth(), Renderer::GetHeight()));

    this->shaderProgram->SetUniform("positionMap", 0);
    this->shaderProgram->SetUniform("normalMap", 1);
    this->shaderProgram->SetUniform("albedoMap", 2);
    this->shaderProgram->SetUniform("mraoMap", 3);

    
    this->shaderProgram->SetUniform("point_light.Kc", Kc);
    this->shaderProgram->SetUniform("point_light.Kl", Kl);
    this->shaderProgram->SetUniform("point_light.Kq", Kq);
}

void PointLight::CalculateRadius() {
   
    float lightMax  = std::fmaxf(std::fmaxf(color.r, color.g), color.b);
    radius    = (-Kl +  std::sqrt(Kl * Kl - 4 * Kq * (Kc - 256.0 * lightMax))) / (2 * Kq);
    model = glm::translate(glm::mat4(1.0f), position_WS)*glm::scale(glm::mat4(1.0f), {radius, radius, radius});
}

void PointLight::SetAttenuation(float k1, float k2, float k3) {
    assert(k1 >= 0 && k2 >= 0 && k3 >=0);
    Kc = k1; 
    Kl = k2; 
    Kq =k3;

    this->shaderProgram->SetUniform("point_light.Kc", Kc);
    this->shaderProgram->SetUniform("point_light.Kl", Kl);
    this->shaderProgram->SetUniform("point_light.Kq", Kq);

    CalculateRadius();
}
void PointLight::SetInnerUniforms() {
    shaderProgram->Run();
    this->shaderProgram->SetUniform("point_light.color", color);
}

void PointLight::Draw(const glm::mat4& projection, const glm::mat4& view) {

    shaderProgram->Run();
    shaderProgram->SetUniform("Model", model);
    shaderProgram->SetUniform("View", view);
    shaderProgram->SetUniform("Projection", projection);
    shaderProgram->SetUniform("point_light.position", glm::vec3(view*glm::vec4(position_WS, 1.0f)));   

    geometry->Draw();
}


void PointLight::StencilPass(const glm::mat4& projection, const glm::mat4& view) {
    stencilProgram->Run();
    stencilProgram->SetUniform("MVP", projection*view*model);
    geometry->Draw();
}

//-------------------------------------------------------------------------------------------------
//
//
/*SpotLight::SpotLight(const glm::vec3& color, const glm::vec3& pos, 
                     const glm::vec3& dir, float angle) 
{
    //this->shaderProgram = Engine::programManager.Get(...);
    //this->geometry = Engine::geometryManager.Get(...);
    this->color = color;
    this->position = pos;
    this->direction = dir;
    this->cutoff = angle;
}

void SpotLight::SetInnerUniforms() const {
    shaderProgram->Run();
    shaderProgram->SetUniform("spot_light.ambientLightColor", color);
    shaderProgram->SetUniform("spot_light.position", position);
    shaderProgram->SetUniform("spot_light.direction", direction);
    shaderProgram->SetUniform("spot_light.cutoff", cutoff);
    shaderProgram->SetUniform("spot_light.exponent", exponent);
}*/

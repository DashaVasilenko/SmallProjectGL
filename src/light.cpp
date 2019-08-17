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

DirectionalLight::DirectionalLight(const glm::vec3& color, const glm::vec3& dir) 
{
    this->shaderProgram = Engine::programManager.Get("data/shaders/directional_light.json");
    this->geometry = Engine::geometryManager.Get("data/quad.obj");

    this->color = color;
    this->direction = -glm::normalize(dir);
    this->shaderProgram->Run();
    
    this->shaderProgram->SetUniform("positionMap", 0);
    this->shaderProgram->SetUniform("normalMap", 1);
    this->shaderProgram->SetUniform("albedoMap", 2);
    this->shaderProgram->SetUniform("mraoMap", 3);
    this->shaderProgram->SetUniform("depthMap", 4);

}

void DirectionalLight::SetInnerUniforms() {
    shaderProgram->Run();
    shaderProgram->SetUniform("directional_light.color", color);
}

void DirectionalLight::Draw(const glm::mat4& view, const glm::mat4& light) {
    shaderProgram->SetUniform("directional_light.direction", glm::normalize(glm::vec3(view*glm::vec4(direction, 0.0f)))); 
    shaderProgram->SetUniform("ViewToLightSpace", light*glm::inverse(view));  

    geometry->Draw();
}

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
    this->debugProgram = Engine::programManager.Get("data/shaders/light_debug.json");
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
    radius = (-Kl + std::sqrt(Kl*Kl - 4*Kq*(Kc - (256.0 / 5.0f)*lightMax))) / (2*Kq);
    model = glm::translate(glm::mat4(1.0f), position_WS)*glm::scale(glm::mat4(1.0f), {radius/10.0f, radius/10.0f, radius/10.0f});
}

void PointLight::SetAttenuation(float k1, float k2, float k3) {
    assert(k1 >= 0 && k2 >= 0 && k3 >=0);
    Kc = k1; 
    Kl = k2; 
    Kq = k3;

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

    shaderProgram->SetUniform("Model", model);
    shaderProgram->SetUniform("View", view);
    shaderProgram->SetUniform("Projection", projection);
    shaderProgram->SetUniform("point_light.position", glm::vec3(view*glm::vec4(position_WS, 1.0f)));   

    geometry->Draw();
}

void PointLight::DebugDraw(const glm::mat4& projection, const glm::mat4& view) {
    debugProgram->Run();
    debugProgram->SetUniform("MVP", projection*view*model);
    debugProgram->SetUniform("color", glm::vec3(1.0f, 1.0f, 0.0f));
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
SpotLight::SpotLight(const glm::vec3& color, const glm::vec3& pos, const glm::vec3& dir, float angle, float intensity, float exponent) {
    this->shaderProgram = Engine::programManager.Get("data/shaders/spot_light.json");
    this->stencilProgram = Engine::programManager.Get("data/shaders/stencil.json");
    this->debugProgram = Engine::programManager.Get("data/shaders/light_debug.json");

    glm::vec3 normalize_dir = glm::normalize(dir);
    this->geometry = Engine::geometryManager.Get("data/light_cone.obj");
    this->color = color*intensity;
    this->position_WS = pos;
    this->direction = normalize_dir;
    this->cutoff = angle;
    this->intensity = intensity;
    this->exponent = exponent;


/* 
    glm::vec3 u = glm::cross({0.0f, -1.0f, 0.0f}, normalize_dir); 
    //float s = glm::length(u); 
    float c = glm::dot({0.0f, -1.0f, 0.0f}, normalize_dir);
    glm::mat3 I = glm::mat3(1.0f);
    glm::mat3 u_x = glm::mat3(0.0f, -u.z, u.y,
                              u.z,  0.0f, -u.x, 
                             -u.y,  u.x,  0.0f);
                        
    //glm::mat3 rotation_matrix = I + u_x + u_x*u_x*((1-c)/(s*s));
    glm::mat4 rotation = I + u_x + u_x*u_x/(1 + c); 
*/

/* 
    float cosine = dot({0.0f, -1.0f, 0.0f}, normalize_dir);
    glm::vec3 v = cross({0.0f, -1.0f, 0.0f}, normalize_dir);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), acosf(cosine), normalize(v));
*/

    glm::vec3 u = glm::cross({0.0f, -1.0f, 0.0f}, normalize_dir);
    float cosine = glm::dot({0.0f, -1.0f, 0.0f}, normalize_dir);
    glm::mat3 u_x = glm::mat3({0.0f, u.z, -u.y},{-u.z, 0.0f, u.x},{u.y, -u.x, 0.0f}); // в glm матрицы задаются по столбцам
    glm::mat4 rotation = glm::mat3(1.0f) + u_x + u_x*u_x/(1 + cosine);
    
/* 
    float radius = atanf(angle);
    this->model = glm::translate(glm::mat4(1.0f), position_WS)*rotation*
                  glm::scale(glm::mat4(1.0f), {intensity, intensity, intensity})*
                  glm::scale(glm::mat4(1.0f), {radius, radius, 1.0f});
*/


    float radius = tanf(glm::radians(angle))*2.0f;
    this->model = glm::translate(glm::mat4(1.0f), position_WS)*rotation*
                  glm::scale(glm::mat4(1.0f), {intensity/2.0f, intensity/2.0f, intensity/2.0f})*
                  glm::scale(glm::mat4(1.0f), {radius, 1.0f , radius});


    //this->model = glm::translate(glm::mat4(1.0f), position_WS)*rotation*glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
    //this->model = glm::translate(glm::mat4(1.0f), position_WS)*rotation*glm::scale(glm::mat4(1.0f), glm::vec3(intensity, intensity, intensity));



    this->shaderProgram->Run();
    this->shaderProgram->SetUniform("ScreenWidthHeight", glm::vec2(Renderer::GetWidth(), Renderer::GetHeight()));
    this->shaderProgram->SetUniform("positionMap", 0);
    this->shaderProgram->SetUniform("normalMap", 1);
    this->shaderProgram->SetUniform("albedoMap", 2);
    this->shaderProgram->SetUniform("mraoMap", 3);




}

void SpotLight::SetInnerUniforms() {
    shaderProgram->Run();
    shaderProgram->SetUniform("spot_light.color", color);
    shaderProgram->SetUniform("spot_light.cutoff", cutoff);
    shaderProgram->SetUniform("spot_light.exponent", exponent);
}

void SpotLight::Draw(const glm::mat4& projection, const glm::mat4& view) {
    shaderProgram->SetUniform("Model", model);
    shaderProgram->SetUniform("View", view);
    shaderProgram->SetUniform("Projection", projection);
    shaderProgram->SetUniform("spot_light.position", glm::vec3(view*glm::vec4(position_WS, 1.0f)));   
    shaderProgram->SetUniform("spot_light.direction", glm::normalize(glm::vec3(view*glm::vec4(direction, 0.0f))));
    geometry->Draw();
}

void SpotLight::DebugDraw(const glm::mat4& projection, const glm::mat4& view) {
    debugProgram->Run();
    debugProgram->SetUniform("MVP", projection*view*model);
    debugProgram->SetUniform("color", glm::vec3(0.0f, 0.0f, 1.0f));
    geometry->Draw();
}

void SpotLight::StencilPass(const glm::mat4& projection, const glm::mat4& view) {
    stencilProgram->Run();
    stencilProgram->SetUniform("MVP", projection*view*model);
    geometry->Draw();
}

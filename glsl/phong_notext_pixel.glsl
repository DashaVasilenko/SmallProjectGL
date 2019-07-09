// Фонг. Все векторы переводяться в пространство камеры, и дальнейшие вычисления ведутся в нем

#version 330 // установка версии GLSL

in vec3 outNormal;
in vec3 outEye;

out vec4 outColor;

struct Material {
    vec3 ambientColor; // отражение фонового света материалом
    vec3 diffuseColor; // отражение рассеянного света материалом
    vec3 specularColor; // отражение зеркального света материалом
    float shininess; // показатель степени зеркального отражения
};

uniform Material material;
vec4 Phong() {

    const vec3 lightPosition = vec3(0.0f, 1.0f, 0.0f);
    const vec3 ambientLightColor = vec3(0.5, 0.5, 0.5); // интенсивность фонового света
    const vec3 diffuseLightColor = vec3(0.5, 0.5, 0.5); // интенсивность рассеянного света
    const vec3 specularLightColor = vec3(0.5, 0.5, 0.5); // интенсивность зеркального света

    vec3 light_direction = normalize(vec3(lightPosition-outEye)); // L
    vec3 eye = normalize(-outEye); // V
    vec3 reflected_light = reflect(-light_direction, outNormal); //R

    vec3 ambient = ambientLightColor*material.ambientColor;
    vec3 diffuse = diffuseLightColor*material.diffuseColor*max(dot(light_direction, outNormal), 0.0f);
    vec3 specular = specularLightColor*material.specularColor*pow(max(dot(eye, reflected_light), 0.0), material.shininess);
    return vec4(ambient + diffuse + specular, 1.0);
}

void main() {
    outColor = Phong();
}
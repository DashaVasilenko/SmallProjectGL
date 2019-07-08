// Фонг. Все векторы переводяться в пространство камеры, и дальнейшие вычисления ведутся в нем

#version 330 // установка версии GLSL

in vec3 outNormal;
in vec3 outEye;
in vec2 outTexCoord;

uniform sampler2D Texture;

out vec4 outColor;

vec4 Phong() {
    const vec3 lightPosition = vec3(0.0f, 1.0f, 0.0f);
    const vec3 ambientLightColor = vec3(0.5, 0.5, 0.5); // интенсивность фонового света
    const vec3 diffuseLightColor = vec3(0.5, 0.5, 0.5); // интенсивность рассеянного света
    const vec3 specularLightColor = vec3(0.5, 0.5, 0.5); // интенсивность зеркального света
 
    const vec3 ambientColor = vec3(0.19225, 0.19225, 0.19225); // отражение фонового света материалом
    const vec3 diffuseColor = vec3(0.50754, 0.50754, 0.50754); // отражение рассеянного света материалом
    const vec3 specularColor = vec3(0.50827, 0.50827, 0.50827); // отражение зеркального света материалом
    const float shininess = 0.40; // показатель степени зеркального отражения

    vec3 light_direction = normalize(vec3(lightPosition-outEye)); // L
    vec3 eye = normalize(-outEye); // V
    vec3 reflected_light = reflect(-light_direction, outNormal); //R

    vec3 ambient = ambientLightColor*ambientColor;
    vec3 diffuse = diffuseLightColor*diffuseColor*max(dot(light_direction, outNormal), 0.0f);
    vec3 specular = specularLightColor*specularColor*pow(max(dot(eye, reflected_light), 0.0), shininess);
    return vec4(ambient + diffuse + specular, 1.0);
}

void main() {
    outColor = texture(Texture, outTexCoord)*Phong();
}
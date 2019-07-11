// Фонг. Все векторы переводяться в пространство камеры, и дальнейшие вычисления ведутся в нем

#version 330 // установка версии GLSL

in vec3 outNormal;
in vec3 outEye;
in vec2 outTexCoord;

struct Material {
    float shininess;
};

uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform Material material;
in vec3 light_direction;
out vec4 outColor;

vec4 Phong() {
    const vec3 ambientLightColor = vec3(0.5, 0.5, 0.5); // интенсивность фонового света
    const vec3 diffuseLightColor = vec3(0.5, 0.5, 0.5); // интенсивность рассеянного света
    const vec3 specularLightColor = vec3(0.5, 0.5, 0.5); // интенсивность зеркального света
 
    vec3 ambientColor = texture(ambientMap, outTexCoord*10.0f).rgb; // отражение фонового света материалом
    vec3 diffuseColor = texture(diffuseMap, outTexCoord*10.0f).rgb; // отражение рассеянного света материалом
    vec3 specularColor = texture(specularMap, outTexCoord*10.0f).rgb; // отражение зеркального света материалом
    // выборка вектора из карты нормалей с областью значений [0,1] и перевод вектора нормали в интервал [-1,1]
    vec3 normal = normalize(2.0*texture(normalMap, outTexCoord*10.0f).rgb - 1.0);

    vec3 eye = normalize(-outEye); // V
    vec3 reflected_light = reflect(-light_direction, normal); //R

    vec3 ambient = ambientLightColor*ambientColor;
    vec3 diffuse = diffuseLightColor*diffuseColor*max(dot(light_direction, normal), 0.0f);
    vec3 specular = specularLightColor*specularColor*pow(max(dot(eye, reflected_light), 0.0), material.shininess);
    return vec4(ambient + diffuse + specular, 1.0);
}

void main() {
    outColor = Phong();
}
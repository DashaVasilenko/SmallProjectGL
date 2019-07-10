// Фонг. Все векторы переводяться в пространство камеры, и дальнейшие вычисления ведутся в нем

#version 330 // установка версии GLSL

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;

out vec3 outNormal;
out vec3 outEye;
out vec2 outTexCoord;
out vec3 light_direction;


void main() {
    vec4 lightDir = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    light_direction = normalize((View*lightDir).xyz);
    outNormal = normalize(NormalMatrix*normal);
    outEye = normalize(vec3(View*Model*vec4(position, 1.0f))); // вектор из глаза
    outTexCoord = texCoord;
    gl_Position = Projection*View*Model*vec4(position, 1.0f);
}
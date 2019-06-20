#version 330 // установка версии GLSL

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 MVP;
uniform mat3 NormalMatrix;

out vec3 outNormal;

void main() {
    outNormal = normalize(NormalMatrix*normal);
    gl_Position = MVP*vec4(position, 1.0f);
}
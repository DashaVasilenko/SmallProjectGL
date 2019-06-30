#version 330 // установка версии GLSL

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;
uniform vec3 cameraPos;

out vec3 outNormal;
out vec3 inEye;

void main() {
    outNormal = normalize(NormalMatrix*normal);
    vec4 worldPosition = Model*vec4(position, 1.0f);
    inEye = normalize(cameraPos - worldPosition.xyz);
    gl_Position = Projection*View*worldPosition;
}
#version 330 // установка версии GLSL

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;

uniform mat4 MVP;

out vec3 color;
void main() {
    color = inColor;
    gl_Position = MVP*vec4(position, 1.0f);
}
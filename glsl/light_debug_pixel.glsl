#version 330 // установка версии GLSL

uniform vec3 color;
out vec4 outColor;
void main() {
    outColor = vec4(color, 1.0f);
}
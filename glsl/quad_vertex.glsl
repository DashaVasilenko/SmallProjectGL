#version 330 core // установка версии GLSL

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 outTexCoord;

void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0); 
    outTexCoord = texCoord;
}  

#version 330 core // установка версии GLSL

layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 texCoord;

uniform mat4 Model;              
uniform mat4 View;               
uniform mat4 Projection; 

void main() {
    gl_Position = Projection*View*Model*vec4(position.x, position.y, position.z, 1.0); 
}
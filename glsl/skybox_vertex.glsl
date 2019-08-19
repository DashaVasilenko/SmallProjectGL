#version 330 core // установка версии GLSL

layout (location = 0) in vec3 position;

uniform mat4 MVP;

out vec3 outTexCoord;

void main() {
    outTexCoord = position;
    vec4 pos = MVP*vec4(position, 1.0);
    gl_Position = pos.xyww;


    //gl_Position = MVP*vec4(position, 1.0); 
}  

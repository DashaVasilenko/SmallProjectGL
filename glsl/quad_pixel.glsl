#version 330 // установка версии GLSL

in vec2 outTexCoord;     

uniform sampler2D map;

out vec4 outColor;

void main() {
    outColor = texture(map, outTexCoord);
}
#version 330 // установка версии GLSL

in vec2 outTexCoord;

out vec4 outColor;

uniform sampler2D screenTexture;

void main() { 
    outColor = texture(screenTexture, outTexCoord);
}
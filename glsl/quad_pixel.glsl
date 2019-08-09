#version 330 // установка версии GLSL

in vec2 outTexCoord;     

uniform sampler2D map;

out vec4 outColor;

void main() {
    float value = texture(map, outTexCoord).r;
    outColor = vec4(vec3(value), 1.0f);
    
}
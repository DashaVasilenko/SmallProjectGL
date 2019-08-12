#version 330 // установка версии GLSL

in vec2 outTexCoord;     

uniform sampler2D map;

out vec4 outColor;

void main() {
    float exposure = 0.8f;
    vec3 color = texture(map, outTexCoord).rgb;
    color = vec3(1.0) - exp(-color * exposure);

    //color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    outColor = vec4(color, 1.0);

}
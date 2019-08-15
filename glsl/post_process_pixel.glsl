#version 330 // установка версии GLSL
layout (location = 0) out vec4 hdrMap;
layout (location = 1) out vec4 brightMap;

in vec2 outTexCoord;     

uniform sampler2D map;

void main() {
    // HDR Begin
    float exposure = 0.4f;
    vec3 color = texture(map, outTexCoord).rgb;
    color = vec3(1.0) - exp(-color * exposure);

    //color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    hdrMap = vec4(color, 1.0f);
    // HDR End

    // Bright

    brightMap = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
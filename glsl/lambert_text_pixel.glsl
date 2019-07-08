#version 330 // установка версии GLSL

in vec3 outNormal;
in vec2 outTexCoord;

uniform sampler2D Texture;

out vec4 outColor;

void main() {
    vec3 color = vec3(0.0, 1.0 , 0.0);
    vec3 directional_light = normalize(vec3(0.0f, 1.0f, 1.0f));
    float kd = clamp(dot(directional_light, outNormal), 0.0f, 1.0f);

    outColor = texture(Texture, outTexCoord)*kd*vec4(color, 1.0);
}
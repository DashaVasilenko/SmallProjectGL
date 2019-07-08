#version 330 // установка версии GLSL

in vec3 outNormal;
in vec3 inEye;
in vec2 outTexCoord;

uniform sampler2D Texture;

out vec4 outColor;

vec4 Phong(float kA, float kD, float kS) {

    vec3 directional_light = normalize(vec3(0.0f, 1.0f, 0.0f)); // L
    vec3 eye = inEye; // V
    vec3 reflected_light = reflect(-directional_light, outNormal); //R

    vec4 ambient = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 diffuse = max(dot(directional_light, outNormal), 0.0f)*vec4(0.0f, 1.0f, 0.0f, 1.0f);
    vec4 specular = pow(max(dot(eye, reflected_light), 0.0), 40)*vec4(1.0f, 1.0f, 1.0f, 1.0f);
    return kA*ambient + kD*diffuse + kS*specular;
}

void main() {
    outColor = texture(Texture, outTexCoord)*Phong(0.1f, 0.2f, 0.7f);
}
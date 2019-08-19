#version 330 core

in vec3 outTexCoord; // вектор направления, таже представляющий трехмерную текстурную координату

uniform samplerCube skybox; // сэмплер для кубической карты

out vec4 outColor;


void main() {
    //outColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    vec3 color = pow(texture(skybox, outTexCoord).rgb, vec3(2.2));
    //outColor = texture(skybox, outTexCoord);
    outColor = vec4(color, 1.0f);
}
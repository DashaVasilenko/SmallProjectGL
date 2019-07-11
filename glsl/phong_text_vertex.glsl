// Фонг. Все векторы переводяться в пространство камеры, и дальнейшие вычисления ведутся в нем

#version 330 // установка версии GLSL

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;

out vec3 outNormal;
out vec3 outEye;
out vec2 outTexCoord;
out vec3 light_direction;


void main() {
    outNormal = normalize(NormalMatrix*normal);
    vec3 tang = normalize(NormalMatrix*tangent);
    vec3 bitang = normalize(NormalMatrix*bitangent);
    
    // матрица преобразования в пространство касательных
    mat3 TBN = mat3(tang.x, bitang.x, outNormal.x, 
                    tang.y, bitang.y, outNormal.y,
                    tang.z, bitang.z, outNormal.z);
    vec4 pos = View*Model*vec4(position, 1.0f);
    outEye = TBN*normalize(position.xyz); // вектор из глаза

    const vec3 lightDir = vec3(0.0f, 1.0f, 0.0f);
    light_direction = normalize(TBN*normalize((View*vec4(lightDir, 0.0f)).xyz)); // L

    outTexCoord = texCoord;
    gl_Position = Projection*pos;
}
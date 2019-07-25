// PBR. Все векторы переводяться в пространство камеры, и дальнейшие вычисления ведутся в нем

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
out vec2 outTexCoord;   
out vec3 outPosition;
out mat3 TBN_inverse;         

void main() {
    outNormal = normalize(NormalMatrix*normal);
    vec3 tang = normalize(NormalMatrix*tangent);
    vec3 bitang = normalize(NormalMatrix*bitangent);
    
    // матрица преобразования из пространства касательных
    TBN_inverse = mat3(tang.x, tang.y, tang.z, 
                       bitang.x, bitang.y, bitang.z,
                       outNormal.x, outNormal.y, outNormal.z);
    vec4 pos = View*Model*vec4(position, 1.0f);
    outPosition = pos.xyz;

    outTexCoord = texCoord;
    gl_Position = Projection*pos;
}
     

    
#version 330 core // установка версии GLSL

layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 texCoord;

uniform mat4 Model;              
uniform mat4 View;               
uniform mat4 Projection; 
//uniform mat3 NormalMatrix;

//out vec2 outTexCoord;

void main() {
    gl_Position = Projection*View*Model*vec4(position.x, position.y, position.z, 1.0); 
    //gl_Position = View*Model*vec4(position.x, position.y, position.z, 1.0);
    //outTexCoord = texCoord;
    //outTexCoord = vec2( 0.5*(gl_Position.x + 1.0), 0.5*(1.0 - gl_Position.y) );
    //outTexCoord = vec2( gl_FragCoord.x/ScreenWidth , gl_FragCoord.y/ScreenHeight );
}  


/*

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;        
layout (location = 2) in vec2 texCoord;      
layout (location = 3) in vec3 tangent;       
layout (location = 4) in vec3 bitangent;

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

    const vec3 lightDir = vec3(0.0f, 1.0f, 0.0f); // направление на источник света
    light_direction = normalize(TBN*normalize((View*vec4(lightDir, 0.0f)).xyz)); // L

    outTexCoord = texCoord;
    gl_Position = Projection*pos;
}

*/
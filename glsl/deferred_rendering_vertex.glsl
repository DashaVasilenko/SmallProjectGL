//#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
#if defined ALBEDO_TEXTURE || defined NORMAL_TEXTURE || defined ROUGHNESS_TEXTURE || defined METALLIC_TEXTURE || defined AO_TEXTURE        
    layout (location = 2) in vec2 texCoord; 
#endif

#ifdef NORMAL_TEXTURE
    layout (location = 3) in vec3 tangent;       
    layout (location = 4) in vec3 bitangent;
#endif

uniform mat4 Model;              
uniform mat4 View;               
uniform mat4 Projection;         
uniform mat3 NormalMatrix;

#ifndef NORMAL_TEXTURE
    out vec3 outNormal;
#else
    vec3 outNormal;
    out mat3 TBN_inverse;
#endif

#if defined ALBEDO_TEXTURE || defined NORMAL_TEXTURE || defined ROUGHNESS_TEXTURE || defined METALLIC_TEXTURE || defined AO_TEXTURE        
    out vec2 outTexCoord;
#endif

out vec3 outPosition;

void main() {
    outNormal = normalize(NormalMatrix*normal);

    #ifdef NORMAL_TEXTURE
        vec3 tang = normalize(NormalMatrix*tangent);
        vec3 bitang = normalize(NormalMatrix*bitangent);
    
        // матрица преобразования из пространства касательных
        TBN_inverse = mat3( tang.x, tang.y, tang.z, 
                            bitang.x, bitang.y, bitang.z,
                            outNormal.x, outNormal.y, outNormal.z);
    #endif

    vec4 pos = View*Model*vec4(position, 1.0f);
    outPosition = pos.xyz;

    #if defined ALBEDO_TEXTURE || defined NORMAL_TEXTURE || defined ROUGHNESS_TEXTURE || defined METALLIC_TEXTURE || defined AO_TEXTURE        
        outTexCoord = texCoord;
    #endif
    gl_Position = Projection*pos;
}
     

    
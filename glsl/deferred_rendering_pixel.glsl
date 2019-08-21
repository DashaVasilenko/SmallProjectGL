//#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMetallRoughnessAO;

#ifndef NORMAL_TEXTURE
    in vec3 outNormal;
#else
    in mat3 TBN_inverse;  
#endif

#if defined ALBEDO_TEXTURE || defined NORMAL_TEXTURE || defined ROUGHNESS_TEXTURE || defined METALLIC_TEXTURE || defined AO_TEXTURE
    in vec2 outTexCoord;
    uniform float texture_scale; 
#endif     
in vec3 outPosition;          

#ifdef ALBEDO_TEXTURE
    uniform sampler2D albedoMap;
#else
    uniform vec3 albedo;
#endif

#ifdef ROUGHNESS_TEXTURE
    uniform sampler2D roughnessMap;
#else
    uniform float roughness;
#endif

#ifdef METALLIC_TEXTURE
    uniform sampler2D metallicMap;
#else
    uniform float metallic;
#endif

#ifdef NORMAL_TEXTURE
    uniform sampler2D normalMap;
#endif

#ifdef AO_TEXTURE
    uniform sampler2D aoMap;
#endif

void main()
{
    // ВЫВОД ВСЕГО В ВИДИМЫХ КООРДИНАТАХ!
    
    gPosition = outPosition;
    
    #if defined ALBEDO_TEXTURE || defined NORMAL_TEXTURE || defined ROUGHNESS_TEXTURE || defined METALLIC_TEXTURE || defined AO_TEXTURE
        vec2 TexCoord = outTexCoord*texture_scale;
    #endif

    #ifdef ALBEDO_TEXTURE
        gAlbedo = texture(albedoMap, TexCoord).rgb;
    #else
        gAlbedo = albedo;
    #endif


    #ifdef NORMAL_TEXTURE
        gNormal = normalize(TBN_inverse*(normalize(2.0*texture(normalMap, TexCoord).rgb - 1.0)));
    #else
        gNormal = outNormal;
    #endif

    #ifdef METALLIC_TEXTURE
        gMetallRoughnessAO.r = texture(metallicMap, TexCoord).r;
    #else
        gMetallRoughnessAO.r = metallic;
    #endif

    #ifdef ROUGHNESS_TEXTURE
        gMetallRoughnessAO.g = texture(roughnessMap, TexCoord).r;
    #else
        gMetallRoughnessAO.g = roughness;
    #endif

    #ifdef AO_TEXTURE
        gMetallRoughnessAO.b = texture(aoMap, TexCoord).r;
    #else
        gMetallRoughnessAO.b = 0.0;
    #endif
}
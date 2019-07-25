#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMetallRoughnessAO;

in vec3 outNormal; 
in vec2 outTexCoord;      
in vec3 outPosition;          
in mat3 TBN_inverse;  

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

void main()
{
    // ВЫВОД ВСЕГО В ВИДИМЫХ КООРДИНАТАХ!
    
    // записываем позицию фрагмента в первую текстуру G-буфера
    gPosition = outPosition;
    // так же записываем уникальную для каждого фрагмента нормаль в G-буфер
    gNormal = normalize(TBN_inverse*(normalize(2.0*texture(normalMap, outTexCoord).rgb - 1.0)));
    // и цвет
    gAlbedo = texture(albedoMap, outTexCoord).rgb;
    gMetallRoughnessAO.r = texture(metallicMap, outTexCoord).r;
    gMetallRoughnessAO.g = texture(roughnessMap, outTexCoord).r;
    gMetallRoughnessAO.b = texture(aoMap, outTexCoord).r;
}
#version 330 // установка версии GLSL        

uniform vec2 ScreenWidthHeight;       

struct PointLight {
    vec3 color; 
    vec3 position;
    float Kc;
    float Kl;
    float Kq;
};

uniform PointLight point_light;

uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D albedoMap;
uniform sampler2D mraoMap;

out vec4 outColor;              

const float PI = 3.14159265359;

vec3 FresnelSchlick(vec3 F0, vec3 h, vec3 eye) {  
    return F0 + (1.0 - F0)*pow(1.0 - max(dot(h, eye), 0.0f), 5.0);
}   

float DistributionGGX(vec3 normal, vec3 h, float roughness) {
    float roughness2 = roughness*roughness;
    float NdotH  = max(dot(normal, h), 0.0);
    float a = (NdotH*NdotH*(roughness2 - 1.0) + 1.0);
    a = PI*a*a;
    return roughness2/a;
}

float GeometrySchlickGGX(vec3 normal, vec3 vector, float roughness) {
    float r = roughness + 1.0; 
    float k = (r*r)/8.0; // k = (roughness*roughness)/2; это для IBL
    float NdotV = max(dot(normal, vector), 0.0); // скалярное произведение нормали с вектором взгляда или света
    float a = NdotV*(1.0 - k) + k;
    return NdotV/a;
}

float GeometrySmith(vec3 normal, vec3 lightDir, vec3 inEye, float roughness) {
    float ggx2  = GeometrySchlickGGX(normal, lightDir, roughness);
    float ggx1  = GeometrySchlickGGX(normal, inEye, roughness);
    return ggx1*ggx2;
} 

vec4 PBR() {	
    vec2 outTexCoord = gl_FragCoord.xy/ScreenWidthHeight;

    vec3 outPos = texture(positionMap, outTexCoord).rgb;
    vec3 outEye = normalize(outPos);
    vec3 light_direction_unnormalized = point_light.position - outPos;
    float distance = length(light_direction_unnormalized);
    float attenuation = 1.0 / (point_light.Kc + distance*(point_light.Kl + point_light.Kq*distance)); //затухание света
    vec3 light_direction = normalize(light_direction_unnormalized);

    vec3 normal = texture(normalMap, outTexCoord).rgb;
    //vec3 albedo = texture(albedoMap, outTexCoord).rgb;
    vec3 albedo = pow(texture(albedoMap, outTexCoord).rgb, vec3(2.2));
    // выборка вектора из карты нормалей с областью значений [0,1] и перевод вектора нормали в интервал [-1,1]
   
    float metallic = texture(mraoMap, outTexCoord).r;
    float roughness = texture(mraoMap, outTexCoord).g;
    float ao = texture(mraoMap, outTexCoord).b;

    vec3 eye = normalize(-outEye); // V 

    vec3 F0 = vec3(0.04); // для неметаллов в среднем берется так 
    F0 = mix(F0, albedo, metallic); // смешиваем, если metallic будет 0.0, то получим 0.04 для неметаллов
	           
    // выражение отражающей способности
    vec3 Lo = vec3(0.0);
    // for(int i = 0; i < cntLight; ++i) // цикл по количеству источников (расчет энергетической яркости для каждого источника света)
    vec3 h = normalize(eye + light_direction); // медианный вектор
 
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, h, roughness); // функция нормального распределения   
    float G = GeometrySmith(normal, eye, light_direction, roughness); // функция геометрии 
    //vec3 F = FresnelSchlick(F0, light_direction, normal); //  уравнение Френеля
    vec3 F = FresnelSchlick(F0, h, eye); //  уравнение Френеля
   
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
        
    vec3 numeratorDFG = NDF*G*F;
    float denominator = 4.0 * max(dot(normal, eye), 0.0) * max(dot(normal, light_direction), 0.0);
    vec3 specular = numeratorDFG / max(denominator, 0.001);  
            
    // прибавляем результат к исходящей энергетической яркости Lo
    float NdotL = max(dot(normal, light_direction), 0.0);     
    vec3 radiance = point_light.color*attenuation*NdotL;

    Lo += (kD*albedo/PI + specular) * radiance * NdotL;   
  
    // добавляем подобие фоновой компоненты освещения к результатам расчета непосредственного источника света  
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = Lo + ambient;
	
    // чтобы избежать потери HDR величин, перед гамма-коррекцией необходимо провести тональную компрессию
    //color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));  
   
    return vec4(color, 1.0);
    //return vec4(texture(albedoMap, outTexCoord).rgb, 1.0);
}   

void main() {
    outColor = PBR();
}
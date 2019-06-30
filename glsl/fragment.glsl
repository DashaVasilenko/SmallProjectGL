#version 330 // установка версии GLSL

in vec3 outNormal;
in vec3 inEye;

out vec4 outColor;

// нормаль, направление источника света, вектор взгляда и параметр, определяющий шероховатость поверхности
vec3 CookTorrance(vec3 normal, vec3 light, vec3 view, float roughness_koef)
{
    float Rs;
    if (roughness_koef <= 0.0) Rs = 0.0;
    vec3  h = normalize(view + light);
    float NdotV = max(dot(normal, view), 0.0);
    float NdotL = max(dot(normal, light), 0.0);
    float NdotH = max(dot(normal, h), 1.0e-7);
    float VdotH = max(dot(view, h), 0.0);

// вычислим геометрическую составляющую
    float geometric = 2.0*NdotH/VdotH;
    geometric = min(1.0, geometric*min(NdotV, NdotL));

// вычислим компонент, учитывающий шероховатость поверхности
    float roughness_2 = roughness_koef*roughness_koef;
    float NdotH_2 = NdotH*NdotH;
    float k = roughness_2*NdotH_2;
    float roughness = exp((NdotH_2 - 1.0)/k)/(4.0*k*NdotH_2);

// вычислим коэффициент Френеля
    float fresnel = 1.0/(1.0 + NdotV);

// вычисляем результат, добавляя к знаменателю малую величину, чтобы не было деления на ноль
    Rs = min(1.0, (fresnel*geometric*roughness)/(NdotV*NdotL+1.0e-7));
// vResult = kA*vAmbient + LdotN *(kD*vDiffuse + kS*vSpecular*Rs)
    //return 0.1f*vec3(0.24725, 0.19950, 0.07450) + NdotL*(0.6f*vec3(0.75164, 0.60648, 0.22648) +0.3f*vec3(0.62828, 0.55580, 0.36606)*Rs);
    return 0.1f*vec3(1.0, 0.0, 0.0) + NdotL*(0.1f*vec3(1.0, 0.0, 0.0) +0.8f*vec3(1.0, 0.0, 0.0)*Rs);
}

void main() {
    outColor = vec4(CookTorrance(outNormal, vec3(0.0f, 1.0f, 0.0f), -inEye, 100), 1.0);
}
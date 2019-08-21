//#version 330

in vec3 outNormal;
in vec3 outEye;
in vec3 outPos;
in mat4 outView;

out vec4 outColor;

struct Material {
    vec3 ambientColor; // отражение фонового света материалом
    vec3 diffuseColor; // отражение рассеянного света материалом
    vec3 specularColor; // отражение зеркального света материалом
    float shininess; // показатель степени зеркального отражения
};
uniform Material material;

struct DirectLight {
    vec3 ambientColor; // интенсивность фонового света
    vec3 diffuseColor; // интенсивность рассеянного света
    vec3 specularColor; // интенсивность зеркального света
    vec3 direction; // направление света
};
uniform DirectLight direct_light;

struct PointLight {
    vec3 ambientLightColor; // интенсивность фонового света
    vec3 diffuseLightColor; // интенсивность рассеянного света
    vec3 specularLightColor; // интенсивность зеркального света
    vec3 position; // позиция света
    float Kc;
    float Kl;
    float Kq;
};
uniform PointLight point_light;

struct SpotLight {
    vec3 ambientLightColor; // интенсивность фонового света
    vec3 diffuseLightColor; // интенсивность рассеянного света
    vec3 specularLightColor; // интенсивность зеркального света
    vec3 position; // позиция света
    vec3 direction;
    float cutoff;
    float exponent;
};
uniform SpotLight spot_light;

vec3 PhongDirectionalLight() {
    // направленный источник света
    vec3 light_direction = normalize( (outView*vec4(-direct_light.direction, 0.0)).xyz); // L для направленного
    vec3 eye = normalize(-outEye); // V
    vec3 reflected_light = reflect(-light_direction, outNormal); //R

    vec3 ambient = direct_light.ambientColor*material.ambientColor;
    vec3 diffuse = direct_light.diffuseColor*material.diffuseColor*max(dot(light_direction, outNormal), 0.0f);
    vec3 specular = direct_light.specularColor*material.specularColor*pow(max(dot(eye, reflected_light), 0.0), material.shininess);

    return vec3(ambient + diffuse + specular);
}

vec3 PhongPointLight() {
    // точечный источник света
    vec3 light_pos = (outView*vec4(point_light.position, 1.0)).xyz;
    vec3 lightdir_unnormalized = light_pos - outPos; // направление на источник света
    vec3 light_direction = normalize(lightdir_unnormalized); // L это для точечного
    vec3 eye = normalize(-outEye); // V
    vec3 reflected_light = reflect(-light_direction, outNormal); //R

    float distance = length(lightdir_unnormalized);
    float attenuation = 1.0 / (point_light.Kc + distance*(point_light.Kl + point_light.Kq*distance)); //затухание света

    vec3 ambient = point_light.ambientLightColor*material.ambientColor*attenuation;
    vec3 diffuse = point_light.diffuseLightColor*material.diffuseColor*max(dot(light_direction, outNormal), 0.0f)*attenuation;
    vec3 specular = point_light.specularLightColor*material.specularColor*pow(max(dot(eye, reflected_light), 0.0), material.shininess)*attenuation;

    return vec3(ambient + diffuse + specular);
}

vec3 PhongSpotLight() {
    // узконаправленный источник света
    vec3 light_pos = (outView*vec4(spot_light.position, 1.0)).xyz;
    vec3 lightdir_unnormalized = light_pos - outPos; // направление на источник света
    vec3 light_direction = normalize(lightdir_unnormalized); // L
 
    vec3 spot_dir = normalize( (outView*vec4(spot_light.direction, 0.0)).xyz); // spot_light.direction в нужной с.к.
    float angle = acos( dot(-light_direction, spot_dir) ); 
    float cutoff = radians(clamp(spot_light.cutoff, 0.0, 90.0)); 

    vec3 ambient = spot_light.ambientLightColor*material.ambientColor;

    if (angle < cutoff) {       
        vec3 eye = normalize(-outEye); // V
        vec3 reflected_light = reflect(-light_direction, outNormal); //R
        float attenuation = pow( dot(-light_direction, spot_dir), spot_light.exponent); //???

        vec3 diffuse = spot_light.diffuseLightColor*material.diffuseColor*max(dot(light_direction, outNormal), 0.0f)*attenuation;
        vec3 specular = spot_light.specularLightColor*material.specularColor*pow(max(dot(eye, reflected_light), 0.0), material.shininess)*attenuation; 
        return vec3(ambient + diffuse + specular);
    }
    else // иначе используем только фоновую компоненту, чтобы сцена не была полностью черной вне конуса прожектора 
        return vec3(ambient);
}

void main() {
    // сделать так, чтобы можно было сделать много источников света
    //в функцию передавать элемент из юниформ массива структур класса
    // возможно, количество источников какого-то типа задавать через юниформу
    vec3 color = vec3(0.0);

    color += PhongDirectionalLight();
    //for(int i = 0; i < cnt_point_lights; i++)
  	//color += PhongPointLight();
    //for(int i = 0; i < cnt_spot_lights; i++)
    //color += PhongSpotLight();

    if (color.x > 1.0) color.x = 1.0;
    if (color.y > 1.0) color.y = 1.0;
    if (color.z > 1.0) color.z = 1.0;
    outColor = vec4(color, 1.0);
}
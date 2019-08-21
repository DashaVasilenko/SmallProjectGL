//#version 330 core

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

vec4 Phong() {

    vec3 light_pos = (outView*vec4(spot_light.position, 1.0)).xyz;
    vec3 lightdir_unnormalized = light_pos - outPos; // направление на источник света
    vec3 light_direction = normalize(lightdir_unnormalized); // L
 
    vec3 spot_dir = normalize( (outView*vec4(spot_light.direction, 0.0)).xyz); // spot_light.direction в нужной с.к.
    float angle = acos( dot(-light_direction, spot_dir) ); 
    float cutoff = radians(clamp(spot_light.cutoff, 0.0, 90.0)); 

    vec3 ambient = spot_light.ambientLightColor*material.ambientColor;

    //float theta = dot(lightDir, normalize(-light.direction));
    if (angle < cutoff) {       
        vec3 eye = normalize(-outEye); // V
        vec3 reflected_light = reflect(-light_direction, outNormal); //R
        float attenuation = pow( dot(-light_direction, spot_dir), spot_light.exponent); //???
        //float attenuation = 1.0f;

        //float theta     = dot(lightDir, normalize(-light.direction));
        //float epsilon   = spot_light.cutoff - light.outerCutOff;
        //float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   

        vec3 diffuse = spot_light.diffuseLightColor*material.diffuseColor*max(dot(light_direction, outNormal), 0.0f)*attenuation;
        vec3 specular = spot_light.specularLightColor*material.specularColor*pow(max(dot(eye, reflected_light), 0.0), material.shininess)*attenuation; 
        return vec4(ambient + diffuse + specular, 1.0);
    }
    else // иначе используем только фоновую компоненту, чтобы сцена не была полностью черной вне конуса прожектора 
        return vec4(ambient, 1.0);
        //return vec4(0.0, 0.0, 0.0, 0.0);
    
    
    /*
    // точечный источник света
    vec3 light_pos = (outView*vec4(point_light.position, 1.0)).xyz;
    vec3 lightdir_unnormalized = light_pos - outPos; // направление на источник света
    vec3 light_direction = normalize(lightdir_unnormalized); // L это для точечного
    vec3 eye = normalize(-outEye); // V
    vec3 reflected_light = reflect(-light_direction, outNormal); //R

    float distance = length(lightdir_unnormalized);

    float attenuation = 1.0 / (point_light.Kc + distance*(point_light.Kl + point_light.Kq*distance)); //затухание света
    //float attenuation = 1.0;

    vec3 ambient = point_light.ambientLightColor*material.ambientColor*attenuation;
    vec3 diffuse = point_light.diffuseLightColor*material.diffuseColor*max(dot(light_direction, outNormal), 0.0f)*attenuation;
    vec3 specular = point_light.specularLightColor*material.specularColor*pow(max(dot(eye, reflected_light), 0.0), material.shininess)*attenuation;

    return vec4(ambient + diffuse + specular, 1.0);
    */

    /*
    // направленный источник света
    vec3 light_direction = normalize(vec3(-direct_light.direction)); // L для направленного
    vec3 eye = normalize(-outEye); // V
    vec3 reflected_light = reflect(-light_direction, outNormal); //R

    vec3 ambient = direct_light.ambientColor*material.ambientColor;
    vec3 diffuse = direct_light.diffuseColor*material.diffuseColor*max(dot(light_direction, outNormal), 0.0f);
    vec3 specular = direct_light.specularColor*material.specularColor*pow(max(dot(eye, reflected_light), 0.0), material.shininess);

    return vec4(ambient + diffuse + specular, 1.0);
    */

    //return vec4(ambient + diffuse + specular, 1.0);
    //return vec4(diffuse + specular, 1.0);
}

void main() {
    outColor = Phong();
}
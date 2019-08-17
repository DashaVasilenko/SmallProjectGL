#version 330 // установка версии GLSL
<<<<<<< HEAD
layout (location = 0) out vec3 hdrMap;
layout (location = 1) out vec3 brightMap;
=======
layout (location = 0) out vec4 hdrMap;
layout (location = 1) out vec4 brightMap;
>>>>>>> ee67200a04908b3af4515e3bd97fe1ddc989e3ce

in vec2 outTexCoord;     

uniform sampler2D map;

<<<<<<< HEAD
//out vec4 outColor;
=======
>>>>>>> ee67200a04908b3af4515e3bd97fe1ddc989e3ce
void main() {
    // HDR Begin
    float exposure = 0.4f;
    vec3 fragColor = texture(map, outTexCoord).rgb;
    vec3 color = vec3(1.0) - exp(-fragColor * exposure);

    //color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
<<<<<<< HEAD
    hdrMap = color;
    //outColor = vec4(color, 1.0f);
=======
    hdrMap = vec4(color, 1.0f);
>>>>>>> ee67200a04908b3af4515e3bd97fe1ddc989e3ce
    // HDR End

    //// Bright
    brightMap = vec3(0.0f, 1.0f, 0.0f);
/*
    // проверка фрагмента на превышение по яркости заданного порога
    // если ярче - вывести в отдельный буфер, хранящий яркие участки
    float brightness = dot(fragColor, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightMap = color;
        //outColor = vec4(color, 1.0);
    else
        brightMap = vec3(0.0, 0.0, 0.0);
        //outColor = vec4(0.0, 0.0, 0.0, 1.0);*/

<<<<<<< HEAD
=======
    brightMap = vec4(0.0f, 1.0f, 0.0f, 1.0f);
>>>>>>> ee67200a04908b3af4515e3bd97fe1ddc989e3ce
}
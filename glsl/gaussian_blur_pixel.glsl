//#version 330 core

in vec2 outTexCoord;     

uniform sampler2D brightMap;

uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

out vec4 outColor;

void main() {
    // получаем размер одного текселя
    vec2 tex_offset = 1.0 / textureSize(brightMap, 0); 
    // вклад текущего фрагмента
    vec3 result = texture(brightMap, outTexCoord).rgb * weight[0]; 

    if(horizontal) {
        for(int i = 1; i < 5; ++i) {
            result += texture(brightMap, outTexCoord + vec2(tex_offset.x*i, 0.0)).rgb * weight[i];
            result += texture(brightMap, outTexCoord - vec2(tex_offset.x*i, 0.0)).rgb * weight[i];
        }
    }
    else {
        for(int i = 1; i < 5; ++i) {
            result += texture(brightMap, outTexCoord + vec2(0.0, tex_offset.y*i)).rgb * weight[i];
            result += texture(brightMap, outTexCoord - vec2(0.0, tex_offset.y*i)).rgb * weight[i];
        }
    }

    outColor = vec4(result, 1.0);
}
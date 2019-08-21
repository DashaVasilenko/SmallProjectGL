//#version 330 core
  
in vec2 outTexCoord;

uniform sampler2D hdrMap;
uniform sampler2D bloomBlur;

out vec4 outColor;

void main() {             
    vec3 hdrColor = texture(hdrMap, outTexCoord).rgb;      
    vec3 bloomColor = texture(bloomBlur, outTexCoord).rgb;
    hdrColor += bloomColor; // additive blending
    outColor = vec4(hdrColor, 1.0);
}
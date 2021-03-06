//#version 330 core

in vec2 outTexCoord;     

//uniform sampler2D hdrMap;
//uniform sampler2D brightMap;

uniform sampler2D themap;


out vec4 outColor;

void main() {
    outColor = texture(themap, outTexCoord);
}
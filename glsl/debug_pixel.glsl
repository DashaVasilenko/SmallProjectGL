//#version 330 core

in vec2 outTexCoord;     
uniform sampler2D tex;

out vec4 outColor;
void main() {
    outColor = texture(tex, outTexCoord);
}
//#version 330 core

out vec4 outColor;

void main() {
    #ifdef GREEN_COLOR
        outColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    #else
        #ifdef RED_COLOR
            outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        #else
            outColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
        #endif
    #endif 
}
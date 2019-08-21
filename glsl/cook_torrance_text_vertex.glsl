//#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;

out vec3 outNormal;
out vec3 outEye;
out vec2 outTexCoord;

void main() {
    outNormal = normalize(NormalMatrix*normal);
    outEye = normalize(vec3(View*Model*vec4(position, 1.0f))); // вектор из глаза
    gl_Position = Projection*View*Model*vec4(position, 1.0f);
    outTexCoord = texCoord;
}
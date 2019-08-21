//#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;

out vec3 outNormal;
out vec2 outTexCoord;

void main() {
    outNormal = normalize(NormalMatrix*normal);
    vec4 worldPosition = Model*vec4(position, 1.0f);
    gl_Position = Projection*View*worldPosition;
    outTexCoord = texCoord;
}
//#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;

out vec3 outNormal;
out vec3 outEye;
out vec3 outPos;
out mat4 outView;

void main() {
    outNormal = normalize(NormalMatrix*normal);
    outPos = vec3(View*Model*vec4(position, 1.0f)); // позиция фрагмента
    outEye = normalize(outPos); // вектор из глаза
    gl_Position = Projection*View*Model*vec4(position, 1.0f);
    outView = View;
}
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjectMat;

out vec4 pos;
out vec3 normal;
out vec2 uv;


void main(){
    gl_Position = u_ProjectMat * u_ViewMat * u_ModelMat * vec4(aPos.xyz, 1.0);
    normal = mat3(transpose(inverse(u_ModelMat))) * aNormal;
    pos = u_ModelMat * vec4(aPos.xyz, 1.0);
    uv = aUv;
}
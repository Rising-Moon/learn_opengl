#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjectMat;

out vec4 pos;
out vec3 normal;


void main(){
    pos = u_ProjectMat * u_ViewMat * u_ModelMat * vec4(aPos.xyz, 1.0);
    normal = aNormal;
    gl_Position = pos;
}
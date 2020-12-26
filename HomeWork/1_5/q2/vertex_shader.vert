#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float u_Offset;

out vec3 v_Color;

void main(){
    gl_Position = vec4(aPos.x + u_Offset, -aPos.y, aPos.z, 1.0);
    v_Color = aColor;
}
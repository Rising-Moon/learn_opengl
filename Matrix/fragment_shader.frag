#version 330 core
out vec4 FragColor;

uniform sampler2D texcoord0;
in vec4 pos;

void main(){
    vec3 lightPos = vec3(4.0, 4.0, 4.0);
    float dis = distance(lightPos, pos.xyz) / 4.0;
    FragColor = vec4(0.8f, 0.6f, 0.4f, 1.0f) * dis;
}
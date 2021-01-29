#version 330 core
out vec4 FragColor;

uniform sampler2D texcoord0;
in vec4 pos;
in vec3 normal;

void main(){
    vec3 lightPos = vec3(4.0, 4.0, 4.0);
    float dis = distance(lightPos, pos.xyz) / 4.0;
    FragColor = vec4(0.8, 0.6, 0.4, 1.0) * dis;
    FragColor = vec4(normal, 1.0);
    //    FragColor = vec4(pos.xyz + vec3(0.5), 1.0);
}
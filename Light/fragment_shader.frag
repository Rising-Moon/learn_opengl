#version 330 core

out vec4 FragColor;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_ToyColor;
uniform vec3 u_EyePos;

in vec4 pos;
in vec3 normal;

void main(){
    vec3 eyePos = u_EyePos;
    vec3 toyColor = u_ToyColor;
    vec3 lightColor = u_LightColor;
    vec3 viewDir = normalize(eyePos - pos.xyz);
    vec3 lightDir = normalize(u_LightPos - pos.xyz);
    vec3 reflectDir =reflect(-lightDir, normal);

    //漫反射
    vec3 normalDir = normalize(normal);
    float diff = dot(normalDir, lightDir) / 2.0 + 0.5;
    vec3 diffuse = diff * lightColor;
    //环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    //镜面反射
    float specularStrength = 1.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * u_ToyColor;

    FragColor = vec4(result, 1.0);
}
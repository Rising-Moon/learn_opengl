#version 330 core

struct Material{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform vec3 u_ToyColor;
uniform vec3 u_EyePos;

in vec4 pos;
in vec3 normal;
in vec2 uv;

void main(){
    vec3 eyePos = u_EyePos;

    vec3 toyColor = u_ToyColor;

    vec3 viewDir = normalize(eyePos - pos.xyz);
    vec3 lightDir = normalize(light.position - pos.xyz);
    vec3 normalDir = normalize(normal);

    vec3 reflectDir =reflect(-lightDir, normal);

    //漫反射
    float diff = max(dot(normalDir, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, uv).xyz;
    //环境光
    vec3 ambient = light.ambient * texture(material.diffuse, uv).xyz;
    //镜面反射
    float specularStrength = 1.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * (1.0-texture(material.specular, uv).xyz);

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}
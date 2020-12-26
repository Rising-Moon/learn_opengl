#version 330 core
out vec4 FragColor;

in vec2 o_UV;

uniform sampler2D texcoord0;

void main(){
    //    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    vec4 imageColor = texture(texcoord0, o_UV);
    FragColor = vec4(o_UV,0.0,1.0);
//    FragColor = imageColor;
}
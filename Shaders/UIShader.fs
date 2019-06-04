#version 330 core
out vec4 FragColor;

in UI_VS_OUT
{
    vec2 texCoord;
} fs_in;

uniform sampler2D uiTex;

void main()
{
    FragColor = texture(uiTex, fs_in.texCoord);
}
#version 330 core
out vec4 FragColor;

in UI_VS_OUT
{
    vec2 texCoord;
} fs_in;

uniform sampler2D textTexture;
uniform vec3 textColor;

void main()
{
    vec2 texCoord = fs_in.texCoord;
    texCoord.y = 1 - texCoord.y;
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textTexture, texCoord).r);
    FragColor = vec4(textColor, 1.0) * sampled;
}
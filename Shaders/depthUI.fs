#version 330 core
out vec4 FragColor;

in UI_VS_OUT
{
    vec2 texCoord;
} fs_in;

uniform sampler2D uiTex;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float near_plane = 0.1;
    float far_plane = 100;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{
    float z = texture(uiTex, fs_in.texCoord).r;
    //z = LinearizeDepth(z);
    //FragColor = vec4(1, 1, 0, 1);
    FragColor = vec4(z, z, z, 1);
}
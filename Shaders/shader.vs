#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTex;
layout (location = 4) in vec3 aTangent;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec4 vertColor;
	vec2 texCoord;
	vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProj;
uniform mat4 lightView;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;  
	vs_out.vertColor = aColor;
	vs_out.texCoord = aTex;

	vs_out.FragPosLightSpace = lightProj * lightView * vec4(vs_out.FragPos, 1.0);

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
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
	mat3 TBN;

	// for parallax occlusion
	vec3 TangentViewDir;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProj;
uniform mat4 lightView;
uniform vec3 viewPos;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;  
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	vec3 B = cross(N, T);
	vs_out.TBN = mat3(T, B, N);

	mat3 tranTBN = transpose(vs_out.TBN);
	vs_out.TangentViewDir = normalize(tranTBN * (viewPos - vs_out.FragPos));

	vs_out.vertColor = aColor;
	vs_out.texCoord = aTex;

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
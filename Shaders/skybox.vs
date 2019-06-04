#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTex;
layout (location = 4) in vec3 aTangent;

out VS_OUT
{
	vec3 texCoord;
} vs_out;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_out.texCoord = aPos;

	vec4 pos = projection * view * vec4(aPos, 1.0f);

	gl_Position = pos.xyww;
}
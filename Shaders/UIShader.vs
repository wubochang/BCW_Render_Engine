#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTex;
layout (location = 4) in vec3 aTangent;

out UI_VS_OUT
{
    vec2 texCoord;
} vs_out;

uniform vec2 screenSize;
uniform vec2 screenPos;
uniform vec2 widthHeight;
uniform float zOrder;

void main()
{
	// screen pos from [0, screenSize]
	vec2 pos;
	pos.x = (screenPos.x + aPos.x * widthHeight.x * 0.5 ) / screenSize.x;
	pos.y = (screenPos.y + aPos.y * widthHeight.y * 0.5 ) / screenSize.y;
	pos -= vec2(0.5, 0.5);
	pos *= 2;
	gl_Position = vec4(pos.x, pos.y, zOrder, 1.0f);
	vs_out.texCoord = aTex;
}
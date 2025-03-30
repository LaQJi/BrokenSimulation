#version 330

layout (location = 0) in vec4 v_Color;
layout (location = 1) in vec2 v_TexCoord;

layout (location = 0) out vec4 Color;


float screenPixelRange()
{
	const float range = 1.0;
	vec2 screenTexSize = vec2(1.0) / fwidth(v_TexCoord);
	return range / dot(screenTexSize, screenTexSize);
}

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	Color = vec4(v_Color.rgb, 1.0);
}

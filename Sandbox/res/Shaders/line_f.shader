#version 330

layout (location = 0) in vec3 v_Color;

layout (location = 0) out vec4 Color;

void main()
{
	Color = vec4(v_Color, 1.0);
}

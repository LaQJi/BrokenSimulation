#version 450

layout (location = 0) out vec4 color;

layout (location = 0) in vec4 v_Color;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in flat float v_TexIndex;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = v_Color;

	int index = int(v_TexIndex);
	if (index >= 0 && index < 32)
	{
		texColor *= texture(u_Textures[index], v_TexCoord);
	}

	if (texColor.a < 0.1)
	{
		discard;
	}

	color = texColor;
}

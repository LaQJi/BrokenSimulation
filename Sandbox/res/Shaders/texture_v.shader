#version 330

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;

layout (std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

layout (location = 0) out vec4 v_Color;
layout (location = 1) out vec2 v_TexCoord;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

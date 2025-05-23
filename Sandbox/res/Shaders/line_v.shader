#version  330

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;

layout (std140, binding = 0) uniform Camera {
	mat4 u_ViewProjection;
};

layout (location = 0) out vec3 v_Color;

void main()
{
	v_Color = a_Color;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

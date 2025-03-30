#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec3 v_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

void main()
{
	// 计算片段位置
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));

	// 计算法线
	v_Normal = u_NormalMatrix * a_Normal;

	// 计算裁剪空间坐标
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

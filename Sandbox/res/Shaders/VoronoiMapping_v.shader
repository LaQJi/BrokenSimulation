#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec3 v_Normal;
out vec3 v_GeometryCenter;
out float v_YMin;
out float v_YMax;


// ��������
uniform vec3 u_GeometryCenter;
uniform float u_YMin;
uniform float u_YMax;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat3 u_NormalMatrix;

void main()
{
	// ����Ƭ��λ��
	v_Position = vec3(u_Model * vec4(a_Position, 1.0));

	// ���㷨��
	v_Normal = u_NormalMatrix * a_Normal;

	// ���㼸������
	v_GeometryCenter = vec3(u_Model * vec4(u_GeometryCenter, 1.0));

	// ����߶���ֵ
	v_YMax = vec3(u_Model * vec4(0.0, u_YMax, 0.0, 1.0)).y;
	v_YMin = vec3(u_Model * vec4(0.0, u_YMin, 0.0, 1.0)).y;

	// ����ü��ռ�����
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

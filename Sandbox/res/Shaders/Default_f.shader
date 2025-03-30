#version 330 core

in vec3 v_Position;
in vec3 v_Normal;

out vec4 fragColor;

// ������ɫ
uniform vec4 u_Color;
// �۲���λ��
uniform vec3 u_ViewPos;
// ��������
uniform float u_Shininess;

// ������ϵ��
uniform float u_AmbientStrength;
// ������ϵ��
uniform float u_DiffuseStrength;
// ���淴��ϵ��
uniform float u_SpecularStrength;

// ��Դ����
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

void main()
{
	fragColor = u_Color;
}

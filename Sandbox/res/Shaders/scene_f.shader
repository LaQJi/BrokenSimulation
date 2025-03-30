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
	// ������
	vec3 ambient = u_AmbientStrength * u_LightColor;

	// ������
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPos - v_Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_DiffuseStrength * diff * u_LightColor;

	// ���淴��
	vec3 viewDir = normalize(u_ViewPos - v_Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
	vec3 specular = u_SpecularStrength * spec * u_LightColor;

	vec3 result = (ambient + diffuse + specular) * u_Color.rgb;
	fragColor = vec4(result, u_Color.a);
}

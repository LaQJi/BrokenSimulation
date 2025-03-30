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
uniform int u_LightCount;
struct Light
{
	int Type;	// 0: ���Դ, 1: ƽ�й�
	vec3 Position;
	vec3 Color;
};
// ���֧��10����Դ
uniform Light u_Lights[10];

// ���㵥����Դ�Ĺ���
vec3 CalculateLight(Light light, vec3 norm, vec3 viewDir, vec3 fragPos)
{
	// ���շ���
	vec3 lightDir;
	float attenuation = 1.0;
	if (light.Type == 1)
	{
		// ƽ�й�
		lightDir = normalize(-light.Position);
	}
	else
	{
		// ���Դ
		lightDir = normalize(light.Position - fragPos);

		// ����˥��
		float distance = length(light.Position - fragPos);
		attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
	}

	// ������
	vec3 ambient = u_AmbientStrength * light.Color;

	// ������
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_DiffuseStrength * diff * light.Color;

	// ���淴��
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
	vec3 specular = u_SpecularStrength * spec * light.Color;

	// �ϲ����
	return (ambient + diffuse + specular) * u_Color.rgb * attenuation;
}

void main()
{
	// ��һ��������
	vec3 norm = normalize(v_Normal);

	// �۲췽��
	vec3 viewDir = normalize(u_ViewPos - v_Position);

	// ��ʼ�����
	vec3 result = vec3(0.0);

	// �������й�Դ�Ĺ���
	for (int i = 0; i < min(u_LightCount, 10); i++)
	{
		result += CalculateLight(u_Lights[i], norm, viewDir, v_Position);
	}

	fragColor = vec4(result, u_Color.a);
}

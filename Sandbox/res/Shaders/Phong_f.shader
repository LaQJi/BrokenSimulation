#version 330 core

in vec3 v_Position;
in vec3 v_Normal;

out vec4 fragColor;

// 物体颜色
uniform vec4 u_Color;
// 观察者位置
uniform vec3 u_ViewPos;
// 材质属性
uniform float u_Shininess;

// 环境光系数
uniform float u_AmbientStrength;
// 漫反射系数
uniform float u_DiffuseStrength;
// 镜面反射系数
uniform float u_SpecularStrength;

// 光源属性
uniform int u_LightCount;
struct Light
{
	int Type;	// 0: 点光源, 1: 平行光
	vec3 Position;
	vec3 Color;
};
// 最多支持10个光源
uniform Light u_Lights[10];

// 计算单个光源的光照
vec3 CalculateLight(Light light, vec3 norm, vec3 viewDir, vec3 fragPos)
{
	// 光照方向
	vec3 lightDir;
	float attenuation = 1.0;
	if (light.Type == 1)
	{
		// 平行光
		lightDir = normalize(-light.Position);
	}
	else
	{
		// 点光源
		lightDir = normalize(light.Position - fragPos);

		// 距离衰减
		float distance = length(light.Position - fragPos);
		attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
	}

	// 环境光
	vec3 ambient = u_AmbientStrength * light.Color;

	// 漫反射
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_DiffuseStrength * diff * light.Color;

	// 镜面反射
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
	vec3 specular = u_SpecularStrength * spec * light.Color;

	// 合并结果
	return (ambient + diffuse + specular) * u_Color.rgb * attenuation;
}

void main()
{
	// 归一化法向量
	vec3 norm = normalize(v_Normal);

	// 观察方向
	vec3 viewDir = normalize(u_ViewPos - v_Position);

	// 初始化结果
	vec3 result = vec3(0.0);

	// 计算所有光源的光照
	for (int i = 0; i < min(u_LightCount, 10); i++)
	{
		result += CalculateLight(u_Lights[i], norm, viewDir, v_Position);
	}

	fragColor = vec4(result, u_Color.a);
}

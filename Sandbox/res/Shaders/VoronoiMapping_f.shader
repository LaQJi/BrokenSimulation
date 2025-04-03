#version 330 core

in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_GeometryCenter;
in float v_YMin;
in float v_YMax;

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

// voronoi种子点信息
uniform int u_NumPoints;
uniform vec3 u_Colors[100];
uniform vec3 u_Points[100];

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

	// 返回光照系数
	return (ambient + diffuse + specular) * attenuation;
}

void main()
{
	// 计算法线
	vec3 norm = normalize(v_Normal);

	// 计算视线方向
	vec3 viewDir = normalize(u_ViewPos - v_Position);

	// 计算光照
	vec3 result = vec3(0.0);

	for (int i = 0; i < min(u_LightCount, 10); i++)
	{
		result += CalculateLight(u_Lights[i], norm, viewDir, v_Position);
	}

	// 计算voronoi颜色
	float minDist = 2.0;
	int closetPoint = 0;

	// atan 范围 [-pi, pi]]
	float theta = atan(v_Position.z - v_GeometryCenter.z, v_Position.x - v_GeometryCenter.x);

	// 转换到[0, 1]
	float normalizedTheta = (theta + 3.14159265359) / (2 * 3.14159265359);

	// 将 y 转换到[0, 1]
	float normalizeY = (v_Position.y - v_YMin) / (v_YMax - v_YMin);

	vec2 fragCoord = vec2(normalizedTheta, normalizeY);

	for (int i = 0; i < min(u_NumPoints, 100); i++)
	{
		vec2 point = u_Points[i].xy;

		if (point.x - fragCoord.x > 0.5)
			point.x -= 1.0;
		else if (fragCoord.x - point.x > 0.5)
			point.x += 1.0;
		
		float dist = distance(fragCoord, point);
		if (dist < minDist)
		{
			minDist = dist;
			closetPoint = i;
		}
	}

	result = result * u_Colors[closetPoint];

	fragColor = vec4(result, u_Color.a);
	//fragColor = vec4(0.0, 0.0, u_LightCount, 1.0);
}

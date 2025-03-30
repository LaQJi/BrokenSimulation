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
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

void main()
{
	// 环境光
	vec3 ambient = u_AmbientStrength * u_LightColor;

	// 漫反射
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPos - v_Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_DiffuseStrength * diff * u_LightColor;

	// 镜面反射
	vec3 viewDir = normalize(u_ViewPos - v_Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
	vec3 specular = u_SpecularStrength * spec * u_LightColor;

	vec3 result = (ambient + diffuse + specular) * u_Color.rgb;
	fragColor = vec4(result, u_Color.a);
}

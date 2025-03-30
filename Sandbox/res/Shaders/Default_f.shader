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
	fragColor = u_Color;
}

#pragma once

#include "ECS/Component.h"

namespace BrokenSim
{
	class CameraComponent : public Component
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};
		CameraComponent(Entity* owner,							// 拥有者
			ProjectionType type = ProjectionType::Perspective,	// 投影类型
			float initialValue = 45.0f,							// 初始值
			float aspectRatio = 16.0f / 9.0f,					// 宽高比
			float nearClip = 0.01f,								// 近裁剪面
			float farClip = 1000.0f);							// 远裁剪面
	
	private:
		// 摄像机参数
		ProjectionType m_ProjectionType;	// 投影类型
		float m_FOV;						// 视野角度
		float m_OrthoSize;					// 正交大小
		float m_AspectRatio;				// 宽高比
		float m_NearClip;					// 近裁剪面
		float m_FarClip;					// 远裁剪面

		float m_Yaw;						// 偏航角
		float m_Pitch;						// 俯仰角
		float m_Roll;						// 翻滚角

		// 焦点
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		// 距离
		float m_Distance = 10.0f;
		// 初始鼠标位置
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		// 鼠标灵敏度
		float m_MouseSensitivity = 0.1f;
	};
}

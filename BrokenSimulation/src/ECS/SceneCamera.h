#pragma once

#include "Core/TimeStep.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace BrokenSim
{
	class SceneCamera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,	// 透视
			Orthographic = 1	// 正交
		};
		SceneCamera(ProjectionType type = ProjectionType::Perspective,			// 投影类型
			float initialValue = 45.0f,											// 初始值
			float aspectRatio = 16.0f / 9.0f,									// 宽高比
			float nearClip = 0.01f,												// 近裁剪面
			float farClip = 1000.0f);											// 远裁剪面

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		// 切换投影类型
		void SwitchProjectionType();

		// 设置视口大小
		void SetViewportSize(float width, float height);

		// 设置投影矩阵
		void SetProjectionMatrix(float typeValue, float aspectRatio, float nearClip, float farClip);
		// 获取投影矩阵
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		// 获取视图矩阵
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		// 获取视图投影矩阵
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		// 获取方向信息
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::quat GetOrientation() const;


	private:
		bool OnMouseScroll(MouseScrolledEvent& e);

		void UpdateProjection();
		void UpdateView();

		glm::vec3 CalculatePosition();

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		void UpdateAlternateValue();


	private:
		// 投影矩阵
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		// 视图矩阵
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		// 视图投影矩阵
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		// 摄像机参数
		ProjectionType m_ProjectionType;	// 投影类型
		float m_FOV;						// 视野角度（透视）
		float m_OrthoSize;					// 正交大小（正交）
		float m_AspectRatio;				// 宽高比
		float m_NearClip;					// 近裁剪面
		float m_FarClip;					// 远裁剪面

		// 欧拉角
		float m_Yaw = 0.0f;					// 偏航角
		float m_Pitch = 0.0f;				// 俯仰角
		float m_Roll = 0.0f;				// 滚转角

		// 摄像机位置
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		// 视口大小
		float m_ViewportWidth = 1280.0f;
		float m_ViewportHeight = 720.0f;

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
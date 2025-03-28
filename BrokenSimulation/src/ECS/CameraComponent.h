#pragma once

#include "ECS/Component.h"

namespace BrokenSim
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(Entity* owner,							// 拥有者
			float fov = 45.0f,									// 视野角度
			float aspectRatio = 16.0f / 9.0f,					// 宽高比
			float nearClip = 0.01f,								// 近裁剪面
			float farClip = 1000.0f);							// 远裁剪面

		// 获取投影矩阵
		glm::mat4 GetProjectionMatrix() const;

		// 获取视图矩阵
		glm::mat4 GetViewMatrix() const;

		// 设置视野角度
		void SetFOV(float fov);
		// 获取视野角度
		float GetFOV() const;

		// 设置宽高比
		void SetAspectRatio(float aspectRatio);
		// 获取宽高比
		float GetAspectRatio() const;

		// 设置近裁剪面
		void SetNearClip(float nearClip);
		// 获取近裁剪面
		float GetNearClip() const;

		// 设置远裁剪面
		void SetFarClip(float farClip);
		// 获取远裁剪面
		float GetFarClip() const;

		// 设置位置
		void SetPosition(const glm::vec3& position);
		// 获取位置
		glm::vec3 GetPosition() const;

		// 设置旋转
		void SetRotation(const glm::vec3& rotation);
		// 获取旋转
		glm::vec3 GetRotation() const;
	
	private:
		// 视野角度
		float m_FOV;
		// 宽高比
		float m_AspectRatio;
		// 近裁剪面
		float m_NearClip;
		// 远裁剪面
		float m_FarClip;

		// 位置
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		// 旋转
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	};
}

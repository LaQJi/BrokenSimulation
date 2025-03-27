#pragma once

#include <string>
#include <vector>

#include "Core/TimeStep.h"
#include "Renderer/Shader.h"
#include "Events/Event.h"

#include <glm/glm.hpp>

namespace BrokenSim
{
	class Object
	{
	public:
		enum class Type
		{
			Empty,		// 空对象
			Model,		// 模型
			Light,		// 光源
			Camera,		// 相机
			Point2D		// 二维点
		};

		virtual ~Object() = default;


		// 更新对象状态
		virtual void OnUpdate(TimeStep ts, std::shared_ptr<Shader>& shader) = 0;
		// 渲染对象
		virtual void OnRender(std::shared_ptr<Shader> shader) = 0;
		// 处理事件
		virtual void OnEvent(Event& e) = 0;


		// 获取唯一标识符
		unsigned int GetID() const;
		// 获取类型
		Type GetType() const;
		// 获取名称
		const std::string& GetName() const;
		// 获取变换矩阵
		const glm::mat4& GetTransform() const;
		// 获取父对象
		Object* GetParent() const;
		// 获取子对象
		const std::vector<std::shared_ptr<Object>>& GetChildren() const;
		// 获取可见性
		const bool GetVisible() const;
		// 获取启用性
		const bool GetEnabled() const;
		// 获取位置
		glm::vec3& GetPosition();
		// 获取旋转
		glm::vec3& GetRotation();
		// 获取缩放
		glm::vec3& GetScale();
		// 获取模型矩阵
		const glm::mat4 GetModelMatrix() const;


		// 设置名称
		void SetName(const std::string& name);
		// 设置父对象
		void SetParent(Object* parent);
		// 添加子对象
		void AddChild(std::shared_ptr<Object> child);
		// 移除子对象，返回释放的ID
		std::vector<unsigned int> RemoveChild(std::shared_ptr<Object> child);
		// 移除所有子对象，返回释放的ID
		std::vector<unsigned int> RemoveAllChildren();
		// 设置可见性
		void SetVisible(bool visible);
		// 设置启用性
		void SetEnabled(bool enabled);
		// 设置位置
		void SetPosition(const glm::vec3& position);
		// 设置旋转
		void SetRotation(const glm::vec3& rotation);
		// 设置缩放
		void SetScale(const glm::vec3& scale);


		// 应用位移
		void ApplyTranslation();
		// 应用旋转
		void ApplyRotation();
		// 应用缩放
		void ApplyScale();

	protected:
		// 唯一标识符
		unsigned int id;
		// 类型
		Type type;

	private:
		// 名称
		std::string m_Name;
		// 变换矩阵
		glm::mat4 m_Transform = glm::mat4(1.0f);
		// 父对象
		Object* m_Parent;
		// 子对象
		std::vector<std::shared_ptr<Object>> m_Children;
		// 是否可见
		bool m_Visible = true;
		// 是否启用
		bool m_Enabled = true;
		// 位置
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		// 旋转
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		// 缩放
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };


		void Translate(const glm::vec3& offset);

		void Rotate(const glm::vec3& axis, float angle);

		void Scale(const glm::vec3& scale);
	};

	static std::shared_ptr<Object> NullObject = nullptr;


	static std::string GetTypeName(Object::Type type)

	{
		switch (type)
		{
		case Object::Type::Empty:
			return "Empty";
		case Object::Type::Model:
			return "Model";
		case Object::Type::Light:
			return "Light";
		case Object::Type::Camera:
			return "Camera";
		case Object::Type::Point2D:
			return "Point2D";
		default:
			return "Unknown";
		}
	}
}

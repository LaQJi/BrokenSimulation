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
			Empty,		// �ն���
			Model,		// ģ��
			Light,		// ��Դ
			Camera,		// ���
			Point2D		// ��ά��
		};

		virtual ~Object() = default;


		// ���¶���״̬
		virtual void OnUpdate(TimeStep ts, std::shared_ptr<Shader>& shader) = 0;
		// ��Ⱦ����
		virtual void OnRender(std::shared_ptr<Shader> shader) = 0;
		// �����¼�
		virtual void OnEvent(Event& e) = 0;


		// ��ȡΨһ��ʶ��
		unsigned int GetID() const;
		// ��ȡ����
		Type GetType() const;
		// ��ȡ����
		const std::string& GetName() const;
		// ��ȡ�任����
		const glm::mat4& GetTransform() const;
		// ��ȡ������
		Object* GetParent() const;
		// ��ȡ�Ӷ���
		const std::vector<std::shared_ptr<Object>>& GetChildren() const;
		// ��ȡ�ɼ���
		const bool GetVisible() const;
		// ��ȡ������
		const bool GetEnabled() const;
		// ��ȡλ��
		glm::vec3& GetPosition();
		// ��ȡ��ת
		glm::vec3& GetRotation();
		// ��ȡ����
		glm::vec3& GetScale();
		// ��ȡģ�;���
		const glm::mat4 GetModelMatrix() const;


		// ��������
		void SetName(const std::string& name);
		// ���ø�����
		void SetParent(Object* parent);
		// ����Ӷ���
		void AddChild(std::shared_ptr<Object> child);
		// �Ƴ��Ӷ��󣬷����ͷŵ�ID
		std::vector<unsigned int> RemoveChild(std::shared_ptr<Object> child);
		// �Ƴ������Ӷ��󣬷����ͷŵ�ID
		std::vector<unsigned int> RemoveAllChildren();
		// ���ÿɼ���
		void SetVisible(bool visible);
		// ����������
		void SetEnabled(bool enabled);
		// ����λ��
		void SetPosition(const glm::vec3& position);
		// ������ת
		void SetRotation(const glm::vec3& rotation);
		// ��������
		void SetScale(const glm::vec3& scale);


		// Ӧ��λ��
		void ApplyTranslation();
		// Ӧ����ת
		void ApplyRotation();
		// Ӧ������
		void ApplyScale();

	protected:
		// Ψһ��ʶ��
		unsigned int id;
		// ����
		Type type;

	private:
		// ����
		std::string m_Name;
		// �任����
		glm::mat4 m_Transform = glm::mat4(1.0f);
		// ������
		Object* m_Parent;
		// �Ӷ���
		std::vector<std::shared_ptr<Object>> m_Children;
		// �Ƿ�ɼ�
		bool m_Visible = true;
		// �Ƿ�����
		bool m_Enabled = true;
		// λ��
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		// ��ת
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		// ����
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

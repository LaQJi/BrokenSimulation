#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <queue>

#include "Scene/Object.h"
#include "Scene/EmptyObject.h"
#include "Scene/ModelObject.h"
#include "Scene/Camera.h"
#include "Core/TimeStep.h"

namespace BrokenSim
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(TimeStep ts, Camera& camera);

		std::shared_ptr<EmptyObject> CreateEmptyObject(const Object::Type& type, const std::string& name = "Empty", Object* parent = nullptr);
		std::shared_ptr<ModelObject> CreateModelObject(const Object::Type& type, const std::string& path, const std::string& name = "Model", Object* parent = nullptr);

		void DeleteObject(std::shared_ptr<Object> object);

		void OnImGuiRender();

	private:
		// ������С��
		struct compare
		{
			bool operator()(const unsigned int& a, const unsigned int& b)
			{
				return a > b;
			}
		};
		// ���ڴ洢���õ�ID�����մ�С�����˳������
		// ��ɾ��һ��Objectʱ������ID���뵽m_AvailableIDs��
		// ������һ��Objectʱ����m_AvailableIDs��ȡ��һ��ID
		// ��ȡ��һ��ID��m_AvailableIDsΪ�գ������һ���µ�ID��m_AvailableIDs
		std::priority_queue<unsigned int, std::vector<unsigned int>, compare> m_AvailableIDs;
		
		// ���ڴ洢���е�Object�����º���Ⱦʱ����m_Objects���������丸����ı任�����������ı任����
		std::unordered_map<unsigned int, std::shared_ptr<Object>> m_Objects;

		std::shared_ptr<Shader> m_Shader;
	};
}

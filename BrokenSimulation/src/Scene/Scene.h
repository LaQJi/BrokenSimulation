#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <queue>

#include "Scene/Object.h"
#include "Scene/EmptyObject.h"
#include "Core/TimeStep.h"

namespace BrokenSim
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(TimeStep ts);
		void OnRender();

		template <typename T>
		void RegisterFactoryFunction(const Object::Type& type);

		template <typename T, typename... Args>
		std::shared_ptr<T> CreateObject(const Object::Type& type, const std::string& name, Args&&... args);

		template <typename T, typename... Args>
		std::shared_ptr<T> CreateChildObject(const Object::Type& type, const std::string& name, EmptyObject* parent, Args&&... args);

		void DeleteObject(std::shared_ptr<Object> object);

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

		// ���ڴ洢����ע���Object�Ĺ�������
		std::unordered_map<Object::Type, std::function<std::shared_ptr<Object>(int, const std::string&)>> m_FactoryFunctionMap;

		// TODO: �ƻ�ֻ������Ϊnullptr��Object�洢��m_Objects�У�����Object�洢���丸���m_Children�У����ں�������
		
		// ���ڴ洢���еĸ�Object��������Ϊnullptr��Object
		std::unordered_map<unsigned int, std::shared_ptr<Object>> m_Objects;
	};
}

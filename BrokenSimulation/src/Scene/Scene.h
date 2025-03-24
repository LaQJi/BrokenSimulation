#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <queue>
#include <any>

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
		virtual ~Scene() = default;

		virtual void OnUpdate(TimeStep ts, Camera& camera) = 0;
		virtual void OnRender() = 0;

		virtual void OnImGuiRender() = 0;

		std::shared_ptr<Object> CreateObject(const Object::Type type, const std::string& name = "", Object* parent = nullptr, const std::vector<std::any>& params = {});
		void DeleteObject(std::shared_ptr<Object>& object);

		bool IsObjectInScene(std::shared_ptr<Object>& object);


	protected:
		// ���ڴ洢���е�Object�����º���Ⱦʱ����m_Objects���������丸����ı任�����������ı任����
		std::unordered_map<unsigned int, std::shared_ptr<Object>> m_Objects;
		// ���ڴ洢��������ɫ��
		std::shared_ptr<Shader> m_Shader;


		void Init(std::shared_ptr<Shader>& shader);

		unsigned int AssignID();
		void FreeID(unsigned int id);

		std::shared_ptr<Object>& GetObject(unsigned int id);

		template <typename T, typename... Args>
		bool RegisterObjectFactory(Object::Type type)
		{
			m_ObjectFactory[type] = [](unsigned id, const std::string& name, Object* parent, const std::vector<std::any>& params)
				{
					return CreateObjectWithParams<T, Args...>(id, name, parent, params);
				};
			return true;
		}


	private:
		// ������С��
		struct compare
		{
			bool operator()(const unsigned int& a, const unsigned int& b)
			{
				return a > b;
			}
		};

		//TODO: �ɿ����滻Ϊһ���������˫�����������ͷ�IDʱ��ID�ؽ���ά������С�ڴ濪��
		
		// ���ڴ洢���õ�ID�����մ�С�����˳������
		// ��ɾ��һ��Objectʱ������ID���뵽m_AvailableIDs��
		// ������һ��Objectʱ����m_AvailableIDs��ȡ��һ��ID
		// ��ȡ��һ��ID��m_AvailableIDsΪ�գ������һ���µ�ID��m_AvailableIDs
		std::priority_queue<unsigned int, std::vector<unsigned int>, compare> m_AvailableIDs;

		std::unordered_map<Object::Type, std::function<std::shared_ptr<Object>(unsigned int id, const std::string& name, Object* parent, const std::vector<std::any>&)>> m_ObjectFactory;

		
		// ��������������
		template <typename T, typename... Args>
		static std::shared_ptr<Object> CreateObjectWithParams(unsigned int id, const std::string& name, Object* parent, const std::vector<std::any>& params)
		{
			if (params.size() != sizeof...(Args))
			{
				BS_CORE_ERROR("Invalid arguments!");
				return nullptr;
			}

			try
			{
				return std::make_shared<T>(id, name, parent, std::any_cast<Args>(params)...);
			}
			catch (const std::bad_any_cast& e)
			{
				BS_CORE_ERROR("Invalid arguments!");
				return nullptr;
			}
		}
	};
}

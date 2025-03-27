#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <queue>
#include <any>

#include "Scene/Object.h"
#include "Scene/EmptyObject.h"
#include "Scene/ModelObject.h"
#include "Scene/Point2DObject.h"
#include "Scene/Camera.h"
#include "Core/TimeStep.h"

namespace BrokenSim
{
	class Scene
	{
	public:
		using ExtraParams = std::vector<std::any>;
		using CreatorFunc = std::function<std::shared_ptr<Object>(unsigned int id, const std::string& name, Object* parent, const ExtraParams& params)>;


		virtual ~Scene() = default;

		virtual void OnUpdate(TimeStep ts, Camera& camera) = 0;
		virtual void OnRender() = 0;

		virtual void OnImGuiRender() = 0;


		virtual std::shared_ptr<Object> CreateModelObject(const std::string& path, const std::string& name, Object* parent) = 0;
		virtual std::shared_ptr<Object> CreateEmptyObject(const std::string& name, Object* parent) = 0;
		virtual std::shared_ptr<Object> CreatePoint2D(const std::string& name) = 0;

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
	};
}
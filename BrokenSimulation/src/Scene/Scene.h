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
		// 用于存储所有的Object，更新和渲染时遍历m_Objects，并根据其父对象的变换矩阵计算自身的变换矩阵
		std::unordered_map<unsigned int, std::shared_ptr<Object>> m_Objects;
		// 用于存储场景的着色器
		std::shared_ptr<Shader> m_Shader;


		void Init(std::shared_ptr<Shader>& shader);

		unsigned int AssignID();
		void FreeID(unsigned int id);

		std::shared_ptr<Object>& GetObject(unsigned int id);


	private:
		// 定义最小堆
		struct compare
		{
			bool operator()(const unsigned int& a, const unsigned int& b)
			{
				return a > b;
			}
		};

		//TODO: 可考虑替换为一个自排序的双向链表，可在释放ID时对ID池进行维护，减小内存开销

		// 用于存储可用的ID，按照从小到大的顺序排列
		// 当删除一个Object时，将其ID加入到m_AvailableIDs中
		// 当创建一个Object时，从m_AvailableIDs中取出一个ID
		// 若取出一个ID后m_AvailableIDs为空，则添加一个新的ID至m_AvailableIDs
		std::priority_queue<unsigned int, std::vector<unsigned int>, compare> m_AvailableIDs;
	};
}
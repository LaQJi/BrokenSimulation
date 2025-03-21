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
		// 定义最小堆
		struct compare
		{
			bool operator()(const unsigned int& a, const unsigned int& b)
			{
				return a > b;
			}
		};
		// 用于存储可用的ID，按照从小到大的顺序排列
		// 当删除一个Object时，将其ID加入到m_AvailableIDs中
		// 当创建一个Object时，从m_AvailableIDs中取出一个ID
		// 若取出一个ID后m_AvailableIDs为空，则添加一个新的ID至m_AvailableIDs
		std::priority_queue<unsigned int, std::vector<unsigned int>, compare> m_AvailableIDs;

		// 用于存储所有注册的Object的工厂函数
		std::unordered_map<Object::Type, std::function<std::shared_ptr<Object>(int, const std::string&)>> m_FactoryFunctionMap;

		// TODO: 计划只将父类为nullptr的Object存储在m_Objects中，其他Object存储在其父类的m_Children中，便于后续加载
		
		// 用于存储所有的根Object，即父类为nullptr的Object
		std::unordered_map<unsigned int, std::shared_ptr<Object>> m_Objects;
	};
}

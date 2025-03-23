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
		
		// 用于存储所有的Object，更新和渲染时遍历m_Objects，并根据其父对象的变换矩阵计算自身的变换矩阵
		std::unordered_map<unsigned int, std::shared_ptr<Object>> m_Objects;

		std::shared_ptr<Shader> m_Shader;
	};
}

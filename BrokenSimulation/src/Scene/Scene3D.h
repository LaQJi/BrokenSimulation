#pragma once

#include "Scene/Scene.h"

namespace BrokenSim
{
	class Scene3D : public Scene
	{
	public:
		Scene3D();
		virtual ~Scene3D();

		virtual void OnUpdate(TimeStep ts, Camera& camera) override;
		virtual void OnRender() override;

		virtual void OnImGuiRender() override;


		// 导入的模型文件中应当只包含一个模型，否则会将所有模型整体视为一个模型（可能会导致生成破碎效果出错）
		virtual std::shared_ptr<Object> CreateModelObject(const std::string& path = "", const std::string& name = "Model", Object* parent = nullptr) override;
		virtual std::shared_ptr<Object> CreateEmptyObject(const std::string& name = "Empty", Object* parent = nullptr) override;
		virtual std::shared_ptr<Object> CreatePoint2D(const std::string& name);
	};
}

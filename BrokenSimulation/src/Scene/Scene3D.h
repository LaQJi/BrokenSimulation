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


		// �����ģ���ļ���Ӧ��ֻ����һ��ģ�ͣ�����Ὣ����ģ��������Ϊһ��ģ�ͣ����ܻᵼ����������Ч������
		virtual std::shared_ptr<Object> CreateModelObject(const std::string& path = "", const std::string& name = "Model", Object* parent = nullptr) override;
		virtual std::shared_ptr<Object> CreateEmptyObject(const std::string& name = "Empty", Object* parent = nullptr) override;
		virtual std::shared_ptr<Object> CreatePoint2D(const std::string& name);
	};
}

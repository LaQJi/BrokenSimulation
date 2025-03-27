#pragma once

#include "Scene/Scene.h"

namespace BrokenSim
{
	class Voronoi2DScene : public Scene
	{
	public:
		Voronoi2DScene();
		~Voronoi2DScene();

		virtual void OnUpdate(TimeStep ts, Camera& camera) override;
		virtual void OnRender() override;
		
		virtual void OnImGuiRender() override;

		void SetViewportSize(float width, float height);

		virtual std::shared_ptr<Object> CreateModelObject(const std::string& path, const std::string& name, Object* parent);
		virtual std::shared_ptr<Object> CreateEmptyObject(const std::string& name, Object* parent);
		virtual std::shared_ptr<Object> CreatePoint2D(const std::string& name) override;

	private:
		std::shared_ptr<VertexArray> m_VertexArray;
	};
}

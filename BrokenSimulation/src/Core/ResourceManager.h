#pragma once

#include <memory>

#include "Renderer/Shader.h"

namespace BrokenSim
{
	class ResourceManager
	{
	public:
		ResourceManager();

		void Initialize();

		std::shared_ptr<Shader> AddShaderResource(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

		std::shared_ptr<Shader> GetShaderResource(const std::string& name);

		std::shared_ptr<Shader> GetDefaultShader();

		std::shared_ptr<Shader> GetVoronoiShader();

		std::shared_ptr<Shader> GetVoronoiMappingShader();

	private:
		// 着色器资源
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShaderResources;
		std::shared_ptr<Shader> m_DefaultShader;
		std::shared_ptr<Shader> m_VoronoiShader;
		std::shared_ptr<Shader> m_VoronoiMappingShader;
	};
}

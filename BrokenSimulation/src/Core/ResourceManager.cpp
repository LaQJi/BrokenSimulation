#include "bspch.h"
#include "Core/ResourceManager.h"

#include "Core/Log.h"

namespace BrokenSim
{
	ResourceManager::ResourceManager()
	{
		Initialize();
	}

	void ResourceManager::Initialize()
	{
		m_DefaultShader = AddShaderResource("Default", "res/Shaders/Default_v.shader", "res/Shaders/Default_f.shader");
		m_VoronoiShader = AddShaderResource("Voronoi", "res/Shaders/Voronoi_v.shader", "res/Shaders/Voronoi_f.shader");
		m_VoronoiMappingShader = AddShaderResource("VoronoiMapping", "res/Shaders/VoronoiMapping_v.shader", "res/Shaders/VoronoiMapping_f.shader");
		AddShaderResource("Phong", "res/Shaders/Phong_v.shader", "res/Shaders/Phong_f.shader");
		AddShaderResource("BlinnPhong", "res/Shaders/BlinnPhong_v.shader", "res/Shaders/BlinnPhong_f.shader");
	}

	std::shared_ptr<Shader> ResourceManager::AddShaderResource(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexPath, fragmentPath);
		m_ShaderResources[name] = shader;
		return shader;
	}

	std::shared_ptr<Shader> ResourceManager::GetShaderResource(const std::string& name)
	{
		auto iter = m_ShaderResources.find(name);
		if (iter != m_ShaderResources.end())
		{
			return iter->second;
		}
		else
		{
			BS_CORE_WARN("Shader resource \' {0} \' not found!", name);
			return GetDefaultShader();
		}
	}

	std::shared_ptr<Shader> ResourceManager::GetDefaultShader()
	{
		return m_DefaultShader;
	}

	std::shared_ptr<Shader> ResourceManager::GetVoronoiShader()
	{
		return m_VoronoiShader;
	}

	std::shared_ptr<Shader> ResourceManager::GetVoronoiMappingShader()
	{
		return m_VoronoiMappingShader;
	}
}

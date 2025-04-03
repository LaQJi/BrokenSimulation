#pragma once

#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

namespace BrokenSim
{
	class Shader
	{
	public:
		Shader(const char* vertexPath, const char* fragmentPath);
		Shader(const std::string vertexPath, const std::string fragmentPath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform2f(const std::string& name, const glm::vec2& vector);
		void SetUniform2f(const std::string& name, float v0, float v1);
		void SetUniform3f(const std::string& name, const glm::vec3& vector);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform4f(const std::string& name, const glm::vec4& vector);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

		unsigned int GetRendererID() const;

		void PrintActiveUniforms() const;

	private:
		unsigned int rendererID;
		std::string vertexFilePath;
		std::string fragmentFilePath;
		mutable std::unordered_map<std::string, int> uniformLocationCache;

		int GetUniformLocation(const std::string& name);
		std::string ParseShader(const char* filepath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	};
}

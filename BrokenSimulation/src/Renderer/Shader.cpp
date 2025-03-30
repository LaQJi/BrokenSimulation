#include "bspch.h"
#include "Core/Log.h"
#include "Renderer/Shader.h"
#include "Utils/Utils.h"

namespace BrokenSim
{
	Shader::Shader(const char* vertexPath, const char* fragmentPath)
		: vertexFilePath(vertexPath), fragmentFilePath(fragmentPath)
	{
		std::string vertexShader = ParseShader(vertexPath);
		std::string fragmentShader = ParseShader(fragmentPath);
		rendererID = CreateShader(vertexShader, fragmentShader);
	}

	Shader::Shader(const std::string vertexPath, const std::string fragmentPath)
		: vertexFilePath(vertexPath), fragmentFilePath(fragmentPath)
	{
		std::string vertexShader = ParseShader(vertexPath.c_str());
		std::string fragmentShader = ParseShader(fragmentPath.c_str());
		rendererID = CreateShader(vertexShader, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(rendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(rendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform1i(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1f(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform2f(const std::string& name, const glm::vec2& vector)
	{
		glUniform2f(GetUniformLocation(name), vector.x, vector.y);
	}

	void Shader::SetUniform2f(const std::string& name, float v0, float v1)
	{
		glUniform2f(GetUniformLocation(name), v0, v1);
	}

	void Shader::SetUniform3f(const std::string& name, const glm::vec3& vector)
	{
		glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
	{
		glUniform3f(GetUniformLocation(name), v0, v1, v2);
	}

	void Shader::SetUniform4f(const std::string& name, const glm::vec4& vector)
	{
		glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	}

	void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	unsigned int Shader::GetRendererID() const
	{
		return rendererID;
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		if (uniformLocationCache.find(name) != uniformLocationCache.end())
		{
			return uniformLocationCache[name];
		}
		int location = glGetUniformLocation(rendererID, name.c_str());
		if (location == -1)
		{
			BS_CORE_ERROR("Warning: uniform '{0}' doesn't exist!", name);
		}
		uniformLocationCache[name] = location;
		return location;
	}

	std::string Shader::ParseShader(const char* filepath)
	{
		std::ifstream file(filepath, std::ios::in);
		if (!file.is_open())
		{
			BS_CORE_ERROR("Failed to open file: {0}", filepath);
			file.close();
			return "";
		}

		std::string line;
		std::string code;

		while (getline(file, line))
		{
			code += line + "\n";
		}

		file.close();
		return code;
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			BS_CORE_ERROR("Failed to compile {0} shader!", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
			BS_CORE_ERROR(message);
			_freea(message);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);

		int result;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)_malloca(length * sizeof(char));
			glGetProgramInfoLog(program, length, &length, message);
			BS_CORE_ERROR("Failed to link shader program!");
			BS_CORE_ERROR(message);
			_freea(message);
			glDeleteProgram(program);
			return 0;
		}

		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}
}

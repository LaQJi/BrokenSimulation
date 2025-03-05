#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
	: vertexFilePath(vertexPath), fragmentFilePath(fragmentPath)
{
	std::string vertexShader = ParseShader(vertexPath);
	std::string fragmentShader = ParseShader(fragmentPath);
	rendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(rendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(rendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& vector)
{
	GLCall(glUniform2f(GetUniformLocation(name), vector.x, vector.y));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& vector)
{
	GLCall(glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& vector)
{
	GLCall(glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
	GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
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
	GLCall(int location = glGetUniformLocation(rendererID, name.c_str()));
	if (location == -1)
	{
		std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}
	uniformLocationCache[name] = location;
	return location;
}

std::string Shader::ParseShader(const char* filepath)
{
	std::ifstream file(filepath, std::ios::in);
	if (!file.is_open())
	{
		std::cerr << "Error: Failed to open file" << std::endl;
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
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cerr << message << std::endl;
        _freea(message);
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));

	int result;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetProgramInfoLog(program, length, &length, message));
		std::cerr << "Failed to link shader program!" << std::endl;
		std::cerr << message << std::endl;
		_freea(message);
		GLCall(glDeleteProgram(program));
		return 0;
	}

	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

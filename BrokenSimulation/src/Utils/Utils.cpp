#include "bspch.h"
#include "Utils/Utils.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace BrokenSim
{
	void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLLogCall(const char* function, const char* file, int line)
	{
		while (GLenum error = glGetError())
		{
			std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
			return false;
		}
		return true;
	}

	float RandomFloat(float min, float max)
	{
		// 生成0.0到1.0之间的随机数
		float random = ((float)rand()) / (float)RAND_MAX;

		// 将随机数映射到min到max之间
		return min + random * (max - min);
	}

	float Time::GetTime()
	{
		return glfwGetTime();
	}
}

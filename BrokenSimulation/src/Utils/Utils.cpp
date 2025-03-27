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
		// ����0.0��1.0֮��������
		float random = ((float)rand()) / (float)RAND_MAX;

		// �������ӳ�䵽min��max֮��
		return min + random * (max - min);
	}

	float Time::GetTime()
	{
		return glfwGetTime();
	}
}

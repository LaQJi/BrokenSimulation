#include "bspch.h"
#include "Core/Timer.h"

namespace BrokenSim
{
	Timer::Timer()
	{
		Reset();
	}

	float Timer::Elapsed() const
	{
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end - start;
		return duration.count();
	}

	void Timer::Reset()
	{
		start = std::chrono::high_resolution_clock::now();
	}
}
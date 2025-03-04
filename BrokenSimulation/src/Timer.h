#pragma once

#include <chrono>

class Timer
{
public:
	Timer();

	float Elapsed() const;
	void Reset();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

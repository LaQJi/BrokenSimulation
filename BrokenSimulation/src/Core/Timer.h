#pragma once

namespace BrokenSim
{
	class Timer
	{
	public:
		Timer();

		float Elapsed() const;
		void Reset();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};
}
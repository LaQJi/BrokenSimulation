#pragma once

namespace BrokenSim
{
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;
		virtual void OnEvent() = 0;
	};
}

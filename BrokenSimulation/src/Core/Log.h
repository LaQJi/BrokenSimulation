#pragma once

#include <algorithm>

#include "Logger/logger.h"
#include "Core/Config.h"

namespace BrokenSim
{
	class BS_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<Logger::Logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<Logger::Logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<Logger::Logger> s_CoreLogger;
		static std::shared_ptr<Logger::Logger> s_ClientLogger;
	};
}

// Core log macros
#define BS_CORE_DEBUG(...)    ::BrokenSim::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define BS_CORE_INFO(...)     ::BrokenSim::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BS_CORE_WARN(...)     ::BrokenSim::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BS_CORE_ERROR(...)    ::BrokenSim::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BS_CORE_FATAL(...)    ::BrokenSim::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define BS_DEBUG(...)         ::BrokenSim::Log::GetClientLogger()->debug(__VA_ARGS__)
#define BS_INFO(...)          ::BrokenSim::Log::GetClientLogger()->info(__VA_ARGS__)
#define BS_WARN(...)          ::BrokenSim::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BS_ERROR(...)         ::BrokenSim::Log::GetClientLogger()->error(__VA_ARGS__)
#define BS_FATAL(...)         ::BrokenSim::Log::GetClientLogger()->fatal(__VA_ARGS__)

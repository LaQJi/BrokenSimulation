#include "Core/Log.h"

namespace BrokenSim
{
	std::shared_ptr<Logger::Logger> Log::s_CoreLogger;
	std::shared_ptr<Logger::Logger> Log::s_ClientLogger;

	void Log::Init()
	{
		std::string date = Logger::Logger::getCurrentDate();
		date.replace(date.begin(), date.end(), "-", "_");
		std::string logFilePath = "logs/bs_" + date + ".log";
		s_CoreLogger = std::make_shared<Logger::Logger>("BROKENSIMULATION", logFilePath);
		s_ClientLogger = std::make_shared<Logger::Logger>("APP", logFilePath);
	}
}

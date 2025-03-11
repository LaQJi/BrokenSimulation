#include "Core/Log.h"

namespace BrokenSim
{
	std::shared_ptr<Logger::Logger> Log::s_CoreLogger;
	std::shared_ptr<Logger::Logger> Log::s_ClientLogger;

	void Log::Init()
	{
		std::string date = Logger::Logger::getCurrentDate();
		std::replace(date.begin(), date.end(), '-', '_');
		std::string logFilePath = "bs_" + date + ".log";
		s_CoreLogger = std::make_shared<Logger::Logger>("CORE", logFilePath);
		s_ClientLogger = std::make_shared<Logger::Logger>("APP", logFilePath);

		s_CoreLogger->setColorMode(true);
		s_ClientLogger->setColorMode(true);
	}
}

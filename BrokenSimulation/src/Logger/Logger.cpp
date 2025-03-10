#include "Logger/Logger.h"

namespace Logger
{
	Logger::Logger()
		: loggerName("Logger"),
		logFilePath(""),
		exportLogToFile(false),
		colorMode(false)
	{
	}

	Logger::Logger(const std::string& loggerName)
		: loggerName(loggerName),
		logFilePath(""),
		exportLogToFile(false),
		colorMode(false)
	{
	}

	Logger::Logger(const std::string& loggerName, const std::string& logFilePath)
		: loggerName(loggerName),
		logFilePath(logFilePath),
		exportLogToFile(true),
		colorMode(false)
	{
	}

	Logger::~Logger()
	{
	}

	void Logger::setColorMode(bool colorMode)
	{
		this->colorMode = colorMode;
	}

	void Logger::setLogFilePath(const std::string& logFilePath)
	{
		this->logFilePath = logFilePath;
		this->exportLogToFile = true;
	}

	void Logger::log(Severity severity, const std::string& message)
	{
		// 格式化日志信息
		std::string log = formatLog(severity, message);

		// 输出到控制台
		if (colorMode)
		{
			std::cout << getColorCode(severity) << log << "\033[0m" << std::endl;
		}
		else
		{
			std::cout << log << std::endl;
		}

		// 输出到文件
		if (exportLogToFile)
		{
			exportLog(log);
		}
	}

	void Logger::debug(const std::string& message)
	{
		log(Severity::DEBUG, message);
	}

	void Logger::info(const std::string& message)
	{
		log(Severity::INFO, message);
	}

	void Logger::warn(const std::string& message)
	{
		log(Severity::WARN, message);
	}

	void Logger::error(const std::string& message)
	{
		log(Severity::ERROR, message);
	}

	void Logger::fatal(const std::string& message)
	{
		log(Severity::FATAL, message);
	}

	const std::string Logger::getCurrentDate()
	{
		time_t now = time(0);
		tm* time = localtime(&now);
		std::stringstream ss;
		ss << time->tm_year + 1900 << "-" << time->tm_mon + 1 << "-" << time->tm_mday;
		return ss.str();
	}

	const std::string Logger::getCurrentTime()
	{
		time_t now = time(0);
		tm* time = localtime(&now);
		std::stringstream ss;
		ss << time->tm_hour << ":" << time->tm_min << ":" << time->tm_sec;
		return ss.str();
	}

	std::string Logger::getSeverityString(Severity severity)
	{
		switch (severity)
		{
		case Severity::DEBUG:
			return "DEBUG";
		case Severity::INFO:
			return "INFO";
		case Severity::WARN:
			return "WARN";
		case Severity::ERROR:
			return "ERROR";
		case Severity::FATAL:
			return "FATAL";
		default:
			return "UNKNOWN";
		}
	}

	std::string Logger::formatLog(Severity severity, const std::string& message)
	{
		std::string log;
		log += "[" + getCurrentDate() + " " + getCurrentTime() + "]" +
			"[" + getSeverityString(severity) + "] " +
			loggerName + ": " + message;
		return log;
	}

	std::string Logger::getColorCode(Severity severity)
	{
		switch (severity)
		{
		case Severity::DEBUG:
			return "\033[36m";	// 蓝色
		case Severity::INFO:
			return "\033[32m";	// 绿色
		case Severity::WARN:
			return "\033[33m";	// 黄色
		case Severity::ERROR:
			return "\033[31m";	// 红色
		case Severity::FATAL:
			return "\033[35m";	// 紫色
		default:
			return "\033[0m";	// 默认
		}
	}

	void Logger::exportLog(const std::string& log)
	{
		std::ofstream file(logFilePath, std::ios::app);
		if (file.is_open())
		{
			file << log << std::endl;
			file.close();
		}
		else
		{
			std::cerr << "ERROR::LOGGER::Failed to open log file" << std::endl;
		}
	}
}

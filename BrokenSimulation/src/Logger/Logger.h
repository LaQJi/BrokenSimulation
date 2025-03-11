#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>

namespace Logger
{
	class Logger
	{
	public:
		enum class Severity
		{
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL
		};

		// 构造函数
		Logger()
			: loggerName("Logger"),
			logFilePath(""),
			exportLogToFile(false),
			colorMode(false),
			minSeverity(Severity::INFO)
		{
		}

		Logger(const std::string& loggerName)
			: loggerName(loggerName),
			logFilePath(""),
			exportLogToFile(false),
			colorMode(false),
			minSeverity(Severity::INFO)
		{
		}

		Logger(const std::string& loggerName, const std::string& logFilePath)
			: loggerName(loggerName),
			logFilePath(logFilePath),
			exportLogToFile(true),
			colorMode(false),
			minSeverity(Severity::INFO)
		{
		}

		~Logger() {}



		// 设置彩色输出
		void setColorMode(bool colorMode)
		{
			this->colorMode = colorMode;
		}


		// 设置最低日志级别
		void setMinSeverity(Severity minSeverity)
		{
			this->minSeverity = minSeverity;
		}


		// 设置日志文件路径
		void setLogFilePath(const std::string& logFilePath)
		{
			this->logFilePath = logFilePath;
			this->exportLogToFile = true;
		}


		// 日志输出函数
		template <typename T>
		void log(Severity severity, T message)
		{
			std::ostringstream oss;
			oss << message;
			log(severity, oss.str());
		}

		void log(Severity severity, const std::string& message)
		{
			// 判断日志级别
			if (severity < minSeverity)
			{
				return;
			}

			// 判断日志级别
			if (severity < minSeverity)
			{
				return;
			}

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

		template <typename... Args>
		void log(Severity severity, const std::string& message, Args... args)
		{
			// 判断日志级别
			if (severity < minSeverity)
			{
				return;
			}

			// 格式化日志信息
			std::string log = formatLog(severity, message, args...);

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


		// 快捷日志输出函数
		// debug
		template <typename T>
		void debug(T message)
		{
			log(Severity::DEBUG, message);
		}

		void debug(const std::string& message)
		{
			log(Severity::DEBUG, message);
		}

		template <typename... Args>
		void debug(const std::string& message, Args... args)
		{
			log(Severity::DEBUG, message, args...);
		}


		// info
		template <typename T>
		void info(T message)
		{
			log(Severity::INFO, message);
		}

		void info(const std::string& message)
		{
			log(Severity::INFO, message);
		}

		template <typename... Args>
		void info(const std::string& message, Args... args)
		{
			log(Severity::INFO, message, args...);
		}


		// warn
		template <typename T>
		void warn(T message)
		{
			log(Severity::WARN, message);
		}

		void warn(const std::string& message)
		{
			log(Severity::WARN, message);
		}

		template <typename... Args>
		void warn(const std::string& message, Args... args)
		{
			log(Severity::WARN, message, args...);
		}

		
		// error
		template <typename T>
		void error(T message)
		{
			log(Severity::ERROR, message);
		}

		void error(const std::string& message)
		{
			log(Severity::ERROR, message);
		}

		template <typename... Args>
		void error(const std::string& message, Args... args)
		{
			log(Severity::ERROR, message, args...);
		}


		// fatal
		template <typename T>
		void fatal(T message)
		{
			log(Severity::FATAL, message);
		}

		void fatal(const std::string& message)
		{
			log(Severity::FATAL, message);
		}

		template <typename... Args>
		void fatal(const std::string& message, Args... args)
		{
			log(Severity::FATAL, message, args...);
		}


		// 获取当前日期
		static const std::string getCurrentDate()
		{
			auto now = std::chrono::system_clock::now();
			auto time = std::chrono::system_clock::to_time_t(now);
			std::tm tm;
			localtime_s(&tm, &time);
			std::stringstream ss;
			ss << tm.tm_year + 1900 << "-" << tm.tm_mon + 1 << "-" << tm.tm_mday;
			return ss.str();
		}
		

		// 获取当前时间
		static const std::string getCurrentTime()
		{
			auto now = std::chrono::system_clock::now();
			auto time = std::chrono::system_clock::to_time_t(now);
			std::tm tm;
			localtime_s(&tm, &time);
			std::stringstream ss;
			ss << tm.tm_hour << ":" << tm.tm_min << ":" << tm.tm_sec;
			return ss.str();
		}

	private:
		std::string loggerName;
		std::string logFilePath;
		bool exportLogToFile;
		bool colorMode;
		Severity minSeverity;


		// 获取日志级别字符串
		std::string getSeverityString(Severity severity)
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

		// 格式化日志信息
		std::string formatLog(Severity severity, const std::string& message)
		{
			std::string log;
			log += "[" + getCurrentDate() + " " + getCurrentTime() + "]" +
				"[" + getSeverityString(severity) + "] " +
				loggerName + ": " + message;
			return log;
		}

		template <typename... Args>
		std::string formatLog(Severity severity, const std::string& message, Args... args)
		{
			std::vector<std::string> argList = { formatArgs(args)... };
			std::string formatMessage = formatString(message, argList);
			std::string log;
			log += "[" + getCurrentDate() + " " + getCurrentTime() + "]" +
				"[" + getSeverityString(severity) + "] " +
				loggerName + ": " + formatMessage;
			return log;
		}


		// 格式化参数
		template <typename T>
		std::string formatArgs(const T& arg)
		{
			return std::to_string(arg);
		}

		std::string formatArgs(const std::string& arg)
		{
			return arg;
		}

		std::string formatArgs(const char* arg)
		{
			return std::string(arg);
		}


		// 格式化字符串
		std::string formatString(const std::string& format, const std::vector<std::string>& argList)
		{
			std::string result = format;

			for (size_t i = 0; i < argList.size(); i++)
			{
				size_t pos = 0;
				std::string placeholder = "{" + std::to_string(i) + "}";

				while ((pos = result.find(placeholder, pos)) != std::string::npos)
				{
					result.replace(pos, placeholder.length(), argList[i]);
					pos += argList[i].length();
				}
			}

			return result;
		}


		// 获取颜色代码
		std::string getColorCode(Severity severity)
		{
			std::string colorCode;
			switch (severity)
			{
			case Severity::DEBUG:
				colorCode = "\033[36m";
				break;
			case Severity::INFO:
				colorCode = "\033[32m";
				break;
			case Severity::WARN:
				colorCode = "\033[33m";
				break;
			case Severity::ERROR:
				colorCode = "\033[31m";
				break;
			case Severity::FATAL:
				colorCode = "\033[35m";
				break;
			default:
				colorCode = "\033[0m";
			}
			return colorCode;
		}


		// 导出日志到文件
		void exportLog(const std::string& log)
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
	};
}

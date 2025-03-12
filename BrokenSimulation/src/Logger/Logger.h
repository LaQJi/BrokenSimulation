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
			Debug,
			Info,
			Warn,
			Error,
			Fatal
		};

		// ���캯��
		Logger()
			: loggerName("Logger"),
			logFilePath(""),
			exportLogToFile(false),
			colorMode(false),
			minSeverity(Severity::Info)
		{
		}

		Logger(const std::string& loggerName)
			: loggerName(loggerName),
			logFilePath(""),
			exportLogToFile(false),
			colorMode(false),
			minSeverity(Severity::Info)
		{
		}

		Logger(const std::string& loggerName, const std::string& logFilePath)
			: loggerName(loggerName),
			logFilePath(logFilePath),
			exportLogToFile(true),
			colorMode(false),
			minSeverity(Severity::Info)
		{
		}

		~Logger() {}



		// ���ò�ɫ���
		void setColorMode(bool colorMode)
		{
			this->colorMode = colorMode;
		}


		// ���������־����
		void setMinSeverity(Severity minSeverity)
		{
			this->minSeverity = minSeverity;
		}


		// ������־�ļ�·��
		void setLogFilePath(const std::string& logFilePath)
		{
			this->logFilePath = logFilePath;
			this->exportLogToFile = true;
		}


		// ��־�������
		template <typename T>
		void log(Severity severity, T& message)
		{
			std::ostringstream oss;
			oss << message;
			log(severity, oss.str());
		}

		void log(Severity severity, const std::string& message)
		{
			// �ж���־����
			if (severity < minSeverity)
			{
				return;
			}

			// �ж���־����
			if (severity < minSeverity)
			{
				return;
			}

			// ��ʽ����־��Ϣ
			std::string log = formatLog(severity, message);

			// ���������̨
			if (colorMode)
			{
				std::cout << getColorCode(severity) << log << "\033[0m" << std::endl;
			}
			else
			{
				std::cout << log << std::endl;
			}

			// ������ļ�
			if (exportLogToFile)
			{
				exportLog(log);
			}
		}

		template <typename... Args>
		void log(Severity severity, const std::string& message, Args... args)
		{
			// �ж���־����
			if (severity < minSeverity)
			{
				return;
			}

			// ��ʽ����־��Ϣ
			std::string log = formatLog(severity, message, args...);

			// ���������̨
			if (colorMode)
			{
				std::cout << getColorCode(severity) << log << "\033[0m" << std::endl;
			}
			else
			{
				std::cout << log << std::endl;
			}

			// ������ļ�
			if (exportLogToFile)
			{
				exportLog(log);
			}
		}


		// �����־�������
		// debug
		template <typename T>
		void debug(T& message)
		{
			log(Severity::Debug, message);
		}

		void debug(const std::string& message)
		{
			log(Severity::Debug, message);
		}

		template <typename... Args>
		void debug(const std::string& message, Args... args)
		{
			log(Severity::Debug, message, args...);
		}


		// info
		template <typename T>
		void info(T& message)
		{
			log(Severity::Info, message);
		}

		void info(const std::string& message)
		{
			log(Severity::Info, message);
		}

		template <typename... Args>
		void info(const std::string& message, Args... args)
		{
			log(Severity::Info, message, args...);
		}


		// warn
		template <typename T>
		void warn(T& message)
		{
			log(Severity::Warn, message);
		}

		void warn(const std::string& message)
		{
			log(Severity::Warn, message);
		}

		template <typename... Args>
		void warn(const std::string& message, Args... args)
		{
			log(Severity::Warn, message, args...);
		}

		
		// error
		template <typename T>
		void error(T& message)
		{
			log(Severity::Error, message);
		}

		void error(const std::string& message)
		{
			log(Severity::Error, message);
		}

		template <typename... Args>
		void error(const std::string& message, Args... args)
		{
			log(Severity::Error, message, args...);
		}


		// fatal
		template <typename T>
		void fatal(T& message)
		{
			log(Severity::Fatal, message);
		}

		void fatal(const std::string& message)
		{
			log(Severity::Fatal, message);
		}

		template <typename... Args>
		void fatal(const std::string& message, Args... args)
		{
			log(Severity::Fatal, message, args...);
		}


		// ��ȡ��ǰ����
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
		

		// ��ȡ��ǰʱ��
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


		// ��ȡ��־�����ַ���
		std::string getSeverityString(Severity severity)
		{
			switch (severity)
			{
			case Severity::Debug:
				return "DEBUG";
			case Severity::Info:
				return "INFO";
			case Severity::Warn:
				return "WARN";
			case Severity::Error:
				return "ERROR";
			case Severity::Fatal:
				return "FATAL";
			default:
				return "UNKNOWN";
			}
		}

		// ��ʽ����־��Ϣ
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


		// ��ʽ������
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


		// ��ʽ���ַ���
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


		// ��ȡ��ɫ����
		std::string getColorCode(Severity severity)
		{
			std::string colorCode;
			switch (severity)
			{
			case Severity::Debug:
				colorCode = "\033[36m";
				break;
			case Severity::Info:
				colorCode = "\033[32m";
				break;
			case Severity::Warn:
				colorCode = "\033[33m";
				break;
			case Severity::Error:
				colorCode = "\033[31m";
				break;
			case Severity::Fatal:
				colorCode = "\033[35m";
				break;
			default:
				colorCode = "\033[0m";
			}
			return colorCode;
		}


		// ������־���ļ�
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

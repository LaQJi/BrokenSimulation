#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>

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
		Logger();
		Logger(const std::string& loggerName);
		Logger(const std::string& loggerName, const std::string& logFilePath);

		~Logger();

		// 设置彩色输出
		void setColorMode(bool colorMode);

		// 设置日志文件路径
		void setLogFilePath(const std::string& logFilePath);

		// 日志输出函数
		void log(Severity severity, const std::string& message);

		// 快捷日志输出函数
		void debug(const std::string& message);
		void info(const std::string& message);
		void warn(const std::string& message);
		void error(const std::string& message);
		void fatal(const std::string& message);

		// 获取当前日期
		static const std::string getCurrentDate();

		// 获取当前时间
		static const std::string getCurrentTime();

	private:
		std::string loggerName;
		std::string logFilePath;
		bool exportLogToFile;
		bool colorMode;

		// 获取日志级别字符串
		std::string getSeverityString(Severity severity);

		// 格式化日志信息
		std::string formatLog(Severity severity, const std::string& message);

		// 获取颜色代码
		std::string getColorCode(Severity severity);

		// 导出日志到文件
		void exportLog(const std::string& log);
	};
}

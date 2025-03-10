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

		// ���캯��
		Logger();
		Logger(const std::string& loggerName);
		Logger(const std::string& loggerName, const std::string& logFilePath);

		~Logger();

		// ���ò�ɫ���
		void setColorMode(bool colorMode);

		// ������־�ļ�·��
		void setLogFilePath(const std::string& logFilePath);

		// ��־�������
		void log(Severity severity, const std::string& message);

		// �����־�������
		void debug(const std::string& message);
		void info(const std::string& message);
		void warn(const std::string& message);
		void error(const std::string& message);
		void fatal(const std::string& message);

		// ��ȡ��ǰ����
		static const std::string getCurrentDate();

		// ��ȡ��ǰʱ��
		static const std::string getCurrentTime();

	private:
		std::string loggerName;
		std::string logFilePath;
		bool exportLogToFile;
		bool colorMode;

		// ��ȡ��־�����ַ���
		std::string getSeverityString(Severity severity);

		// ��ʽ����־��Ϣ
		std::string formatLog(Severity severity, const std::string& message);

		// ��ȡ��ɫ����
		std::string getColorCode(Severity severity);

		// ������־���ļ�
		void exportLog(const std::string& log);
	};
}

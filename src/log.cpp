//
// Created by xcx on 2023/12/14.
//

#include "log.h"
#include "config.h"
#include <sstream>

Logger::Logger(const std::string &fileName) {
	logFile.open(fileName, std::ios::app);	// 打开文件进行追加写入
	if (!logFile.is_open()) {
		std::cerr << "Failed to open log file: " << fileName << std::endl;
	}
}
Logger::~Logger() {
	if (logFile.is_open()) {
		logFile.close();  // 关闭文件流
	}
}

std::string Logger::GetCurrentTime() {
	// 获取当前时间
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);

	// 将时间转换为 tm 结构
	std::tm *ptm = std::localtime(&now_time);

	// 使用 stringstream 进行格式化
	std::stringstream ss;
	ss << std::put_time(ptm, "%Y-%m-%d %H:%M:%S");

	return ss.str();
}

std::string getLevelStr(Logger::Level level) {
	switch (level) {
		case Logger::Level::INFO:
			return "INFO";
		case Logger::Level::WARNING:
			return "WARNING";
		case Logger::Level::ERROR:
			return "ERROR";
		default:
			return "UNKNOWN";
	}
}

std::string Logger::logTranslateFormat(const std::string &en, const std::string &zh) {
	std::ostringstream oss;
	oss << std::left << en << "\t" << zh << "\n";

	std::string logMessage = "[" + GetCurrentTime() + "]" + oss.str();

	return logMessage;
}

std::string Logger::logTranslateFormat(const std::string &msg, Logger::Level level) {
	std::string logMessage = '[' + GetCurrentTime() + ']' + getLevelStr(level) + ": " + msg + '\n';
	return logMessage;
}

void Logger::logTemplate(const std::string &message, Logger::Level level) {
	std::string logMessage = logTranslateFormat(message, level);

	// 打印到控制台
	// std::cout << logMessage;

	// 写入到文件
	if (logFile.is_open()) {
		logFile << logMessage;
	}
}

void Logger::info(const std::string &en, const std::string &zh) {
	std::string logMsg = logTranslateFormat(en, zh);

	if (logFile.is_open()) {
		logFile << logMsg;
	}
}

void Logger::info(const std::string &message) {
	logTemplate(message, INFO);
}

void Logger::warning(const std::string &message) {
	logTemplate(message, WARNING);
}

void Logger::error(const std::string &message) {
	logTemplate(message, ERROR);
}

void Logger::error(const std::string &en, const std::string &zh) {
	std::string logMsg = logTranslateFormat(en, zh);
	if (logFile.is_open()) {
		logFile << logMsg;
	}
}
void Logger::msg(const std::string &line) {
	if (logFile.is_open()) {
		std::string msg = '[' + GetCurrentTime() + ']' + line + '\n';
		logFile << msg;
	}
}

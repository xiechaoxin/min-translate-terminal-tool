//
// Created by xcx on 2023/12/14.
//

#ifndef TRANSLATEAPP_LOG_H
#define TRANSLATEAPP_LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

class Logger {
public:
	// 日志级别
	enum Level {
		INFO,
		WARNING,
		ERROR
	};

private:
	std::ofstream logFile; // 文件流对象

public:
	// 构造函数
	Logger(const std::string& fileName) {
		logFile.open(fileName, std::ios::app); // 打开文件进行追加写入
		if (!logFile.is_open()) {
			std::cerr << "Failed to open log file: " << fileName << std::endl;
		}
	}

	// 析构函数
	~Logger() {
		if (logFile.is_open()) {
			logFile.close(); // 关闭文件流
		}
	}

	// 获取当前时间的字符串表示
	static std::string GetCurrentTime() {
		auto now = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		std::string timeStr = std::ctime(&now_time);
		timeStr.pop_back(); // 移除换行符
		return timeStr;
	}

	// 输出日志的方法
	void Log(const std::string& message, const std::string& input, const std::string& output, Level level = INFO) {
		std::string levelStr;
		switch (level) {
			case INFO:    levelStr = "[INFO] "; break;
			case WARNING: levelStr = "[WARNING] "; break;
			case ERROR:   levelStr = "[ERROR] "; break;
		}
		std::string logMessage = GetCurrentTime() + " " + levelStr + "Message: " + message
								 + ", Input: " + input + ", Output: " + output + "\n";

		// 打印到控制台
		std::cout << logMessage;

		// 写入到文件
		if (logFile.is_open()) {
			logFile << logMessage;
		}
	}
};

#endif	//TRANSLATEAPP_LOG_H

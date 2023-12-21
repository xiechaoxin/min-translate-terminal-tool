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
	void logTemplate(const std::string &message, Logger::Level level);
	std::string logTranslateFormat(const std::string &en, const std::string &ch);
	std::string logTranslateFormat(const std::string &msg, Level level);


public:
	// 构造函数
	Logger(const std::string& fileName) ;

	// 析构函数
	~Logger();

	// 获取当前时间的字符串表示
	static std::string GetCurrentTime() ;

	// normal msg
	void msg(const std::string& message);

	// 输出日志的方法
	void info(const std::string& message);
	void warning(const std::string& message);
	void error(const std::string& message);

	// for translate log
	void error(const std::string &en, const std::string &zh);
	void info(const std::string &en, const std::string &zh);
};

#endif	//TRANSLATEAPP_LOG_H

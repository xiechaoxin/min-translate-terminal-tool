//
// Created by xcx on 2023/12/14.
//

#ifndef TRANSLATEAPP_CONFIG_H
#define TRANSLATEAPP_CONFIG_H

// 是否区分大小写   true:区分  false:不区分
#define CASE_SENSITIVE false
// 接收的单词最大长度(byte)
#define MAX_WORD_LENGTH 100
// 候选单词数量
#define CANDIDATES_NUMBER 10
// 字典文件路径, 推荐使用绝对路径，如果相对路径，请注意程序的搜索路径从(shell执行目录)开始
#define DB_PATH "../db/dictionary.txt"
#define INVERTED_DB_PATH "../db/inverted-index.txt"

// 最高权重
#define TOP_WEIGHT 100
// 日志相关

// 日志文件路径
#define LOG_PATH "../log.txt"
// 日志级别 DEBUG, INFO, WARNING, ERROR, FATAL
#define LOG_LEVEL Logger::Level::DEBUG

// 是否允许多词翻译 true:允许 false:不允许
#define ALLOW_MULTI_WORD_TRANSLATION true

#endif	//TRANSLATEAPP_CONFIG_H

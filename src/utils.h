#ifndef _UTILS_H_
#define _UTILS_H_

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define LOG_RED(x) (ANSI_COLOR_RED x ANSI_COLOR_RESET)
#define LOG_GREEN(x) (ANSI_COLOR_GREEN x ANSI_COLOR_RESET)
#define LOG_YELLOW(x) (ANSI_COLOR_YELLOW x ANSI_COLOR_RESET)
#define LOG_BLUE(x) (ANSI_COLOR_BLUE x ANSI_COLOR_RESET)
#define LOG_MAGENTA(x) (ANSI_COLOR_MAGENTA x ANSI_COLOR_RESET)
#define LOG_CYAN(x) (ANSI_COLOR_CYAN x ANSI_COLOR_RESET)

namespace utils {

void copyToClipboard(const std::string &text);

bool is_ascii(const std::string &word);

std::string removePunctuation(const std::string &word);

std::string to_lowers(const std::string &word);

template <typename T>
void printVector(const std::vector<T> &vec);

// 蛇形命名，驼峰，下划线命名，分割以上的单词变成单词列表
std::vector<std::string> splitWord(const std::string &word);

// 高亮显示指定文本的函数(无边界判定)
std::string highlightTxt(const std::string &text, const std::string &txtToHighlight);

void printDictionary(const std::string &key);

void printDictionary(const std::vector<std::string> &keys);

void highlightPrintDictionary(const std::string &key, const std::string &needHighlight);

void highlightPrintDictionary(const std::vector<std::string> &keys, const std::string &needHighlight);

void loggerSearch(const std::string &key, const std::string &searchKey);
void loggerSearch(const std::vector<std::string> &res, const std::string &searchKey);
// void loggerSearch(const std::vector<std::string> &res, const std::vector<std::string> &searchKeys);

}  // namespace utils
#endif /* _UTILS_H_ */

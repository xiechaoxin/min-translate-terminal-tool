#ifndef _SEARCH_H_
#define _SEARCH_H_

#pragma once

#include "dictionary.h"
#include <vector>
#include <string>


/// @brief 字符串编辑距离算法
/// @param word1 
/// @param word2 
/// @return 让word1==word2所需的最少编辑距离
int minDistance(const std::string &word1, const std::string &word2);

void processInput(const std::string &input);



std::vector<std::string> fuzzySearch(const std::string &input);



#endif /* _SEARCH_H_ */

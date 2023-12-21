#include "utils.h"
#include "dictionary.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <regex>
#include "log.h"
#include "config.h"

bool utils::is_ascii(const std::string &word) {
	for (const auto &item : word) {
		if (!isascii(item)) {
			return false;
		}
	}
	return true;
}

std::string utils::to_lowers(const std::string &word) {
	std::string lower_case_word = word;
	std::transform(lower_case_word.begin(), lower_case_word.end(), lower_case_word.begin(),
				   [](unsigned char c) { return std::tolower(c); });
	return lower_case_word;
}

void utils::loggerSearch(const std::string &key, const std::string &searchKey) {
	Logger logger(LOG_PATH);
	logger.msg("----------search: [[" + searchKey + "]]----------");
	std::string en = dictionary.at(key).en;
	std::string zh = dictionary.at(key).zh;
	logger.info(en, zh);
}

void utils::loggerSearch(const std::vector<std::string> &res, const std::string &searchKey) {
	Logger logger(LOG_PATH);
	logger.msg("----------search: [[" + searchKey + "]]----------");
	int max_length = 0;
	for (const auto &item : res) {
		max_length = std::max(max_length, static_cast<int>(item.length()));
	}
	for (const auto &item : res) {
		std::string en = dictionary.at(item).en;
		std::string zh = dictionary.at(item).zh;
		zh = utils::highlightTxt(zh, searchKey);
		logger.info(en, zh);
	}
}

// void utils::loggerSearch(const std::vector<std::string> &res, const std::vector<std::string> &searchKeys) {
// 	Logger logger(LOG_PATH);
// 	int max_length = 0;
// 	for (const auto &item : res) {
// 		max_length = std::max(max_length, static_cast<int>(item.length()));
// 	}
// 	for (const auto &item : res) {
// 		std::string en = dictionary.at(item).en;
// 		std::string zh = dictionary.at(item).zh;
// 		zh = utils::highlightTxt(zh, searchKey);
// 		logger.info(en, zh);
// 	}
// }

void utils::printDictionary(const std::string &key) {
	std::string en = dictionary.at(key).en;
	std::string zh = dictionary.at(key).zh;
	printf("%s\t%s\n", en.c_str(), zh.c_str());
}

void utils::printDictionary(const std::vector<std::string> &vec) {
	int max_length = 0;
	for (const auto &item : vec) {
		max_length = std::max(max_length, static_cast<int>(item.length()));
	}
	for (const auto &item : vec) {
		std::string en = dictionary.at(item).en;
		std::string zh = dictionary.at(item).zh;

		printf("%-*s\t%s\n", max_length, en.c_str(), zh.c_str());

		// logger.info(en, zh);
	}
	printf("-------------------------------------------------------------\n");
}

std::string utils::removePunctuation(const std::string &word) {
	std::string result;
	for (char ch : word) {
		if (!std::ispunct(ch)) {
			result += ch;
		}
	}
	return result;
}

std::string utils::highlightTxt(const std::string &text, const std::string &txtToHighlight) {
	// 添加特定的前缀和后缀来突出显示文本
	const std::string HIGHLIGHT_START = "[[";
	const std::string HIGHLIGHT_END = "]]";

	size_t startPos = text.find(txtToHighlight);
	if (startPos != std::string::npos) {
		return text.substr(0, startPos) + HIGHLIGHT_START + txtToHighlight + HIGHLIGHT_END +
			   text.substr(startPos + txtToHighlight.length());
	}
	return text;
}

void utils::highlightPrintDictionary(const std::string &key, const std::string &needHighlight) {
	std::string en = dictionary.at(key).en;
	std::string zh = dictionary.at(key).zh;
	std::string resPrint;
	std::stringstream txtStream;
	txtStream << en << '\t' << zh;
	resPrint = highlightTxt(txtStream.str(), needHighlight);
	std::cout << resPrint << std::endl;
}

void utils::highlightPrintDictionary(const std::vector<std::string> &keys, const std::string &needHighlight) {
	int max_length = 0;
	for (const auto &item : keys) {
		max_length = std::max(max_length, static_cast<int>(item.length()));
	}
	for (const auto &item : keys) {
		std::string en = dictionary.at(item).en;
		std::string zh = dictionary.at(item).zh;
		std::string resPrint;
		std::stringstream txtStream;

		// printf("%-*s\t%s\n", max_length, en.c_str(), zh.c_str());
		txtStream << std::left << std::setw(max_length) << en << "\t" << zh;
		resPrint = highlightTxt(txtStream.str(), needHighlight);
		std::cout << resPrint << '\n';
	}
	printf("-------------------------------------------------------------\n");
}

std::vector<std::string> utils::splitWord(const std::string &word) {
	std::vector<std::string> result;
	std::regex re("([A-Z]?[a-z]+)|([A-Z][A-Z]+(?=[A-Z][a-z]|$))|([0-9]+)");
	std::smatch match;

	std::string::const_iterator searchStart(word.cbegin());
	while (std::regex_search(searchStart, word.cend(), match, re)) {
		result.push_back(match.str(0));	 // 只添加匹配的整个字符串
		searchStart = match.suffix().first;
	}

	return result;
}

template <typename T>
void printVector(const std::vector<T> &vec) {
	for (const auto &elem : vec) {
		std::cout << elem << '\n';
	}
	std::cout << std::endl;
}

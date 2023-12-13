#include "utils.h"
#include "dictionary.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <vector>
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

std::string utils::to_lowers(std::string &word) {
	std::string lower_case_word = word;
	std::transform(lower_case_word.begin(), lower_case_word.end(),
				   lower_case_word.begin(),
				   [](unsigned char c) { return std::tolower(c); });
	return lower_case_word;
}

void utils::printDictionary(const std::string &key) {
	std::string en = dictionary.at(key).en;
	std::string zh = dictionary.at(key).zh;

	printf("%s\t%s\n",
		   en.c_str(),
		   zh.c_str());

	Logger logger(LOG_PATH);
	logger.info(en, zh);

}

void utils::printDictionary(std::vector<std::string> vec) {
	int max_length = 0;
	for (const auto &item : vec) {
		max_length = std::max(max_length, static_cast<int>(item.length()));
	}
	for (const auto &item : vec) {
		std::string en = dictionary.at(item).en;
		std::string zh = dictionary.at(item).zh;

		printf("%-*s\t%s\n",
			   max_length,
			   en.c_str(),
			   zh.c_str());

		Logger logger(LOG_PATH);
		logger.info(en, zh);

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

template <typename T>
void printVector(const std::vector<T> &vec) {
	for (const auto &elem : vec) {
		std::cout << elem << '\n';
	}
	std::cout << std::endl;
}

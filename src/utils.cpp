#include "utils.h"
#include "dictionary.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <vector>

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
	printf("%s\t%s\n",
		   dictionary.at(key).en.c_str(),
		   dictionary.at(key).zh.c_str());

}

void utils::printDictionary(std::vector<std::string> vec) {
	int max_length = 0;
	for (const auto &item : vec) {
		max_length = std::max(max_length, static_cast<int>(item.length()));
	}
	for (const auto &item : vec) {
		printf("%-*s\t%s\n",
			   max_length,
			   dictionary.at(item).en.c_str(),
			   dictionary.at(item).zh.c_str());
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

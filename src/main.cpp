#include <iostream>
#include <string>
#include "dictionary.h"
#include "search.h"
#include "config.h"

void process() {
	std::string input;

	Trie *trie = init_db();

	while (true) {
		std::cout << "请输入文本（回车结束输入, 如果输入为空, 程序结束, "
					 "您也可以手动结束<Ctrl-c>）：\n";
		std::getline(std::cin, input);
		if (input.empty()) {
			break;
		}
		if (input.size() > MAX_WORD_LENGTH) {
			std::cout << "单词长度超过" << MAX_WORD_LENGTH << ", 请重新输入"
					  << "\n";
			continue;
		}
		std::istringstream iss(input);
		std::vector<std::string> words;
		std::string word;

		// 使用空格分割字符串
		while (iss >> word) {
			if (utils::is_ascii(word)) {
				for (const auto &item : utils::splitWord(word)) {
					words.push_back(utils::to_lowers(item));
				}
			} else {
				words.push_back(word);
			}
		}
		if (words.size() == 1) {
			processInput(trie, words[0]);
		} else {
			processInput(trie, words);
		}
	}

	std::cout << "输入结束。\n";
}

int main() {
	process();
	return 0;
}

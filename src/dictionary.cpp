#include "dictionary.h"

std::unordered_map<std::string, std::string> dictionary;
void init_db() {
	std::string line;
	std::ifstream file("../dictionary.txt");  // 假设字典文件名为 dictionary.txt

	if (!file.is_open()) {
		std::cerr << "无法打开文件" << std::endl;
		return;
	}

	while (std::getline(file, line)) {
		std::stringstream linestream(line);
		std::string englishWord, chineseMeaning;

		// 读取英文单词和中文解释
		std::getline(linestream, englishWord, '\t');
		std::getline(linestream, chineseMeaning);

		// 将它们插入到字典中
		dictionary[englishWord] = chineseMeaning;
	}
	file.close();

	// 测试：打印字典内容
	for (const auto &pair : dictionary) {
		if (is_ascii(pair.first)) {
			std::cout << "【ASCII】";
			std::cout << pair.first << std::endl;
		} else {
			std::cout << "【非ASCII】";
			std::cout << pair.first << std::endl;
		}

		if (is_ascii(pair.second)) {
			std::cout << "【ASCII】";
			std::cout << pair.second << std::endl;
		} else {
			std::cout << "【非ASCII】";
			std::cout << pair.second << std::endl;
		}
	}
}

#include "dictionary.h"
#include "trie_tree.h"
#include "utils.h"
#include "config.h"

std::unordered_map<std::string, en_zh> dictionary;
InvertedIndex invered_index; // u_map<str, vec<WordEntry>>

void buildInvertedIndex() {
	std::ifstream file(INVERTED_DB_PATH);
	std::string line;

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string key;
		std::getline(iss, key, '\t');  // 读取键（直到遇到第一个制表符）

		std::string word;
		int weight = TOP_WEIGHT;  // 初始权重，根据需要调整
		while (std::getline(iss, word, '\t')) {  // 读取单词
			invered_index[word].push_back({key, weight--});
		}
	}
}

Trie *buildDictionary() {
	std::string line;
	std::ifstream file(DB_PATH);

	if (!file.is_open()) {
		std::cerr << "无法打开文件" << std::endl;
		return nullptr;
	}

	Trie *trie = new Trie();

	while (std::getline(file, line)) {
		std::stringstream lineStream(line);
		std::string enWord, chWord;

		// 读取英文单词和中文解释
		std::getline(lineStream, enWord, '\t');
		std::getline(lineStream, chWord);

		std::string en_key = utils::removePunctuation(enWord);
		if (CASE_SENSITIVE) {
			// 将它们插入到字典中
			dictionary[en_key] = en_zh(enWord, chWord);
			trie->insert(en_key);
		} else {
			std::string en_key_lower = utils::to_lowers(en_key);
			// 将它们插入到字典中
			dictionary[en_key_lower] = en_zh(enWord, chWord);
			trie->insert(en_key_lower);
		}
	}
	file.close();

	return trie;

	// 测试：打印字典内容
	// for (const auto &pair : dictionary) {
	// 	std::cout << pair.first << " 对应的中文是: " << pair.second << std::endl;
	// }
}

Trie *init_db() {
	buildInvertedIndex();
	return buildDictionary();
}

#include "search.h"
#include "trie_tree.h"
#include "config.h"
#include "log.h"

#define INSERT_COST 1
#define CHANGE_COST 2
#define DELETE_COST 4

/// @brief 返回编辑成本
/// @param word1
/// @param word2
/// @return
int minDistance(const std::string &word1, const std::string &word2) {
	int len1 = word1.size();
	int len2 = word2.size();
	std::vector<int> dp(len2 + 1);

	for (int i = 1; i <= len2; i++) {
		dp[i] = INSERT_COST * i;
	}

	// 动态规划计算编辑距离
	for (int i = 1; i <= len1; i++) {
		int pre = dp[0];
		dp[0] = DELETE_COST * i;
		for (int j = 1; j <= len2; j++) {
			int temp = dp[j];
			if (word1[i - 1] == word2[j - 1]) {
				dp[j] = pre;
			} else {
				dp[j] = std::min({pre + CHANGE_COST, dp[j - 1] + INSERT_COST,
								  temp + DELETE_COST});
			}
			pre = temp;
		}
	}
	return dp[len2];
}
// void processInput(Trie *trie, std::string &input) {}
int fuzzyLimitCost = 2;	 // edit / delete / change cost = 1

void processInput(Trie *trie, std::string &word) {
	std::string key;
	if (utils::is_ascii(key)) {	 // 查找字典中的英文单词
		if (CASE_SENSITIVE) {
			key = word;
		} else {
			key = utils::to_lowers(word);
		}
		std::vector<std::string> res = trie->fuzzySearch(key);
		if (res.empty()) {
			std::cout << "字典中未找到单词 " << key << std::endl;
			Logger logger(LOG_PATH);
			logger.error(key, "字典中未找到单词");
			return;
		}
		utils::printDictionary(res);
	} else {  // 查找字典中的中文解释to_lower
		/* TODO<2023-12-14, @xcx> 中文逻辑比较难，trieTree不好实现，是否可以用其他数据结构？UTF8存储是个问题 */
		std::vector<std::string> res = trie->fuzzySearch(key);
		if (res.empty()) {
			std::cout << "字典中未找到中文解释 " << key << std::endl;
			return;
		}
		utils::printDictionary(res);
	}
}

void processInput(Trie *trie, std::vector<std::string> &words) {
	for (auto &word : words) {
		std::string key;
		if (utils::is_ascii(key)) {	 // 查找字典中的英文单词
			if (CASE_SENSITIVE) {
				key = word;
			} else {
				key = utils::to_lowers(word);
			}
			bool exist = trie->search(key);
			if (!exist) {
				std::cout << key << "\t字典中未找到单词 " << std::endl;
				Logger logger(LOG_PATH);
				logger.error(key, "字典中未找到单词");
				continue;
			}
			utils::printDictionary(word);
		} else {  // 查找字典中的中文解释to_lower
			/* TODO<2023-12-14, @xcx> 中文逻辑比较难，trieTree不好实现，是否可以用其他数据结构？UTF8存储是个问题 */
			std::vector<std::string> res = trie->fuzzySearch(key);
			if (res.empty()) {
				std::cout << "字典中未找到中文解释 " << key << std::endl;
				return;
			}
			utils::printDictionary(res);
		}
	}
}

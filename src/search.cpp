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
				dp[j] = std::min({pre + CHANGE_COST, dp[j - 1] + INSERT_COST, temp + DELETE_COST});
			}
			pre = temp;
		}
	}
	return dp[len2];
}

std::vector<std::string> sortByWeight(std::vector<WordEntry> &res) {
	// 按照 weight 从高到低对 res 进行排序
	std::sort(res.begin(), res.end(), [](const WordEntry &a, const WordEntry &b) {
		return a.weight > b.weight;	 // 降序排序
	});

	std::vector<std::string> res_print;
	// 遍历排序后的 res，并将 key 插入到 res_print 中
	for (const auto &entry : res) {
		res_print.push_back(entry.key);
	}
	return res_print;
}

void processInput(Trie *trie, std::string &searchWord) {
	std::string key;
	if (utils::is_ascii(searchWord)) {  // 查找字典中的英文单词
		if (CASE_SENSITIVE) {
			key = searchWord;
		} else {
			key = utils::to_lowers(searchWord);
		}
		std::vector<std::string> res = trie->fuzzySearch(key);
		if (res.empty()) {
			std::cout << "字典中未找到单词 " << key << std::endl;
			Logger logger(LOG_PATH);
			logger.error(key, "字典中未找到单词");
			return;
		}
		utils::printDictionary(res);
		utils::loggerSearch(res, searchWord);
	} else {  // 查找字典中的中文解释to_lower
		std::vector<WordEntry> res;
		std::vector<std::string> res_print;
		try {
			res = invered_index.at(searchWord);
		} catch (const std::out_of_range &e) {
			std::cout << "\t字典中未找到" << searchWord << "中文解释"
					  << "\n";
			return;
		}
		if (res.empty()) {
			std::cout << "\t字典中未找到" << searchWord << "中文解释"
					  << "\n";
			return;
		}
		if (res.size() > CANDIDATES_NUMBER) {
			res.resize(CANDIDATES_NUMBER);
		}
		res_print = sortByWeight(res);
		utils::highlightPrintDictionary(res_print, searchWord);
		utils::loggerSearch(res_print, searchWord);
	}
}

void processInput(Trie *trie, std::vector<std::string> &searchWords) {
	for (auto &searchWord : searchWords) {
		std::string key;
		if (utils::is_ascii(searchWord)) {  // 查找字典中的英文单词
			if (CASE_SENSITIVE) {
				key = searchWord;
			} else {
				key = utils::to_lowers(searchWord);
			}
			bool exist = trie->search(key);
			if (!exist) {
				std::cout << key << "\t字典中未找到单词 " << std::endl;
				Logger logger(LOG_PATH);
				logger.error(key, "字典中未找到单词");
				continue;
			}
			utils::printDictionary(searchWord);
			utils::loggerSearch(searchWord, searchWord);
		} else {  // 查找字典中的中文解释
			std::vector<WordEntry> res;
			std::vector<std::string> res_print;
			try {
				res = invered_index.at(searchWord);
			} catch (const std::out_of_range &e) {
				std::cout << "\t字典中未找到" << searchWord << "中文解释"
						  << "\n";
				continue;
			}
			if (res.empty()) {
				std::cout << "\t字典中未找到" << searchWord << "中文解释"
						  << "\n";
				continue;
			}
			if (res.size() > CANDIDATES_NUMBER) {
				res.resize(CANDIDATES_NUMBER);
			}
			res_print = sortByWeight(res);
			utils::highlightPrintDictionary(res_print, searchWord);
			utils::loggerSearch(res_print, searchWord);
		}
	}
}

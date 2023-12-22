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

/// @return 如果需要复制，则返回 id，否则返回 -1
int askForCopy() {
	std::cout << "是否需要复制到剪切板, 输入号码复制，空白跳过" << '\n';
	std::string input_id;
	std::getline(std::cin, input_id);
	int idx = -1;
	if (input_id.empty()) {
		return idx;
	}
	// 验证输入是否全为数字
	for (char c : input_id) {
		if (!std::isdigit(c)) {
			std::cout << "输入非法，必须是数字。" << '\n';
			return -1;
		}
	}
	try {
		idx = std::stoi(input_id);
	} catch (const std::exception& e) {
		std::cout << "输入转换错误: " << e.what() << '\n';
		return -1;
	}
	if (idx < 0 ||  idx >= CANDIDATES_NUMBER) {
		return -1;
	}
	return idx;
}

void processEnglishInput(Trie *trie, const std::string &searchWord) {
	std::string key = CASE_SENSITIVE ? searchWord : utils::to_lowers(searchWord);
	std::vector<std::string> res = trie->fuzzySearch(key);
	if (res.empty()) {
		std::cout << "字典中未找到单词 " << key << std::endl;
		Logger logger(LOG_PATH);
		logger.error(key, "字典中未找到单词");
		return;
	}
	utils::printDictionary(res);
	utils::loggerSearch(res, searchWord);
	int id = askForCopy();
	if (id != -1) {
		utils::copyToClipboard(res[id]);
	}
}

void processChineseInput(const std::string &searchWord) {
	std::vector<WordEntry> res;
	try {
		res = invertedIndex.at(searchWord);
	} catch (const std::out_of_range &e) {
		std::cout << "\t字典中未找到" << searchWord << "中文解释\n";
		return;
	}
	if (res.empty()) {
		std::cout << "\t字典中未找到" << searchWord << "中文解释\n";
		return;
	}
	if (res.size() > CANDIDATES_NUMBER) {
		res.resize(CANDIDATES_NUMBER);
	}
	std::vector<std::string> resPrint = sortByWeight(res);
	utils::highlightPrintDictionary(resPrint, searchWord);
	utils::loggerSearch(resPrint, searchWord);
	int id = askForCopy();
	if (id != -1) {
		utils::copyToClipboard(resPrint[id]);
	}
}

void processInput(Trie *trie, const std::string &searchWord) {
	if (utils::is_ascii(searchWord)) {
		processEnglishInput(trie, searchWord);
	} else {
		processChineseInput(searchWord);
	}
}

void processInput(Trie *trie, const std::vector<std::string> &searchWords) {
	for (const auto &searchWord : searchWords) {
		processInput(trie, searchWord);
	}
}


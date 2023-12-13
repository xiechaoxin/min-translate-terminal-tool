#include "search.h"

#define INSERT_COST 1
#define CHANGE_COST 1
#define DELETE_COST 1

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
				dp[j] = std::min({
					pre + CHANGE_COST,
					dp[j - 1] + INSERT_COST,
					temp + DELETE_COST
				});
			}
			pre = temp;
		}
	}
	return dp[len2];
}

void processInput(const std::string &input) {
	if (is_ascii(input)) {
		// 查找字典中的英文单词
		if (dictionary.find(input) != dictionary.end()) {
			std::cout << input << " 对应的中文是: " << dictionary[input]
					  << std::endl;
		} else {
			std::cout << "字典中未找到单词 " << input << std::endl;
		}
	} else {
		// 查找字典中的中文解释
		bool found = false;
		for (const auto &pair : dictionary) {
			if (pair.second == input) {
				std::cout << pair.first << " 对应的中文是: " << pair.second
						  << std::endl;
				found = true;
				break;
			}
		}
		if (!found) {
			std::cout << "字典中未找到中文解释 " << input << std::endl;
		}
	}
}

std::vector<std::string> fuzzySearch(const std::string &input) {
	return {"hello", "world"};
}

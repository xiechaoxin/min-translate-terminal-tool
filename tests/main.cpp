#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int levenshteinDistance(const std::string &s1, const std::string &s2) {
    int len1 = s1.size();
    int len2 = s2.size();
    std::vector<std::vector<int>> d(len1 + 1, std::vector<int>(len2 + 1));

    for (int i = 0; i <= len1; i++) d[i][0] = i;
    for (int j = 0; j <= len2; j++) d[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + cost });
        }
    }

    return d[len1][len2];
}


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

int main() {
    std::string str1 = "你好世界";
    std::string str2 = "你好宇宙";

    std::cout << "Levenshtein Distance: " << levenshteinDistance(str1, str2) << std::endl;
    std::cout << "Levenshtein Distance: " << minDistance(str1, str2) << std::endl;

    return 0;
}

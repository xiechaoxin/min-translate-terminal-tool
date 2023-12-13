/*
 * [72] 编辑距离
 * 给你两个单词 word1 和 word2,  请返回将 word1 转换成 word2
 * 所使用的最少操作数  .
 * 你可以对一个单词进行如下三种操作:
 *
 * 插入一个字符
 * 删除一个字符
 * 替换一个字符
 *
 * 示例 1:
 * 输入: word1 = "horse", word2 = "ros"
 * 输出: 3
 * 解释:
 * horse -> rorse (将 'h' 替换为 'r')
 * rorse -> rose (删除 'r')
 * rose -> ros (删除 'e')
 *
 * 示例 2:
 * 输入: word1 = "intention", word2 = "execution"
 * 输出: 5
 * 解释:
 * intention -> inention (删除 't')
 * inention -> enention (将 'i' 替换为 'e')
 * enention -> exention (将 'n' 替换为 'x')
 * exention -> exection (将 'n' 替换为 'c')
 * exection -> execution (插入 'u')
 *
 * 提示:
 *
 * 0 <= word1.length, word2.length <= 500
 * word1 和 word2 由小写英文字母组成
 *
 * @lc app=leetcode.cn id=72 lang=cpp
 *
 * [72] Edit Distance
 *
 * https://leetcode.cn/problems/edit-distance/description/
 *
 * algorithms
 * Hard (62.77%)
 * Likes:    2722
 * Dislikes: 0
 * Total Accepted:    329.1K
 * Total Submissions: 524.3K
 * Testcase Example:  '"horse"\n"ros"'
 *
 * Given two strings word1 and word2, return the minimum number of operations
 * required to convert word1 to word2.
 *
 * You have the following three operations permitted on a word:
 *
 * Insert a character
 * Delete a character
 * Replace a character
 *
 * Example 1:
 * Input: word1 = "horse", word2 = "ros"
 * Output: 3
 * Explanation:
 * horse -> rorse (replace 'h' with 'r')
 * rorse -> rose (remove 'r')
 * rose -> ros (remove 'e')
 *
 * Example 2:
 * Input: word1 = "intention", word2 = "execution"
 * Output: 5
 * Explanation:
 * intention -> inention (remove 't')
 * inention -> enention (replace 'i' with 'e')
 * enention -> exention (replace 'n' with 'x')
 * exention -> exection (replace 'n' with 'c')
 * exection -> execution (insert 'u')
 *
 * Constraints:
 * 0 <= word1.length, word2.length <= 500
 * word1 and word2 consist of lowercase English letters.
 */
#include <iostream>
#include <string>
#include <vector>

template <typename T>
using Vec = std::vector<T>;

using String = std::string;

class SolutionA {
public:
	int minDistance(String word1, String word2) {
		return minDistancePro(word1, word2, 1, 1, 1);
	}

	/**
     * @brief word1 转换成 word2 需要多少花费
     * @param word1
     * @param word2
     * @param insertCost
     * @param deleteCost
     * @param replaceCost
     * @return
     */

	int minDistancePro(String &word1, String &word2, int insertCost,
					   int deleteCost, int replaceCost) {
		int w1_len = word1.size() + 1;
		int w2_len = word2.size() + 1;

		// dp[i][j] means the edit distance between word1[0..i-1] and word2[0..j-1]

		Vec<Vec<int>> dp(w1_len, Vec<int>(w2_len, 0));

		// the edit distance between word1[0..i-1] and word2[0..0] is i
		for (int i = 1; i < w1_len; i++) {
			dp[i][0] = deleteCost * i;
		}
		// the edit distance between word1[0..0] and word2[0..j-1] is j
		for (int j = 1; j < w2_len; j++) {
			dp[0][j] = insertCost * j;
		}

		for (int i = 1; i < w1_len; i++) {
			for (int j = 1; j < w2_len; j++) {
				if (word1[i - 1] == word2[j - 1]) {
					// word1[0..i-1] and word2[0..j-1] are the same
					// the edit distance between word1[0..i-1] and word2[0..j-1] is the
					// same as word1[0..i-2] and word2[0..j-2]
					dp[i][j] = dp[i - 1][j - 1];
				} else {
					dp[i][j] = dp[i - 1][j - 1] + replaceCost;
				}
				dp[i][j] = std::min(dp[i][j], dp[i][j - 1] + insertCost);
				dp[i][j] = std::min(dp[i][j], dp[i - 1][j] + deleteCost);
			}
		}
		return dp[w1_len - 1][w2_len - 1];
	}
};

static int insertCost = 1;
static int changeCost = 1;
static int deleteCost = 1;
// * 空间压缩
int minDistance(const std::string &word1, const std::string &word2) {
	int w1len = word1.size();
	int w2len = word2.size();
	std::string longs = w1len >= w2len ? word1 : word2;
	std::string shorts = w1len < w2len ? word1 : word2;

	if (w1len < w2len) {
		int tmp = insertCost;
		insertCost = deleteCost;
		deleteCost = tmp;
	}

	std::vector<int> dp(shorts.size() + 1);

	for (int i = 1; i <= shorts.size(); i++) {
		dp[i] = insertCost * i;
	}
	for (int i = 1; i <= longs.size(); i++) {
		int pre = dp[0];
		dp[0] = deleteCost * i;
		for (int j = 1; j < shorts.size(); j++) {
			int tmp = dp[j];
			if (longs[i - 1] == shorts[j - 1]) {
				dp[j] = pre;
			} else {
				dp[j] = pre + changeCost;
			}
			dp[j] = std::min(dp[j], dp[j - 1] + insertCost);
			dp[j] = std::min(dp[j], tmp + deleteCost);
			pre = tmp;
		}
	}
	return dp[shorts.length()];
}

int main(int argc, char const *argv[]) {
	std::string str1 = "你好世界";
	std::string str2 = "djsafda你好宇";
	std::string str3 = "djda你好世界宇";
	SolutionA sola;
	auto res = sola.minDistance(str1, str2);
	std::cout << res << std::endl;

	auto res2 = sola.minDistance(str1, str3);
	std::cout << res2 << std::endl;
	return 0;
}

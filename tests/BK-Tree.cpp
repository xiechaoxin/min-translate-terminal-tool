#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>

// 计算两个字符串之间的 Levenshtein 编辑距离
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
				dp[j] = std::min({pre + CHANGE_COST, dp[j - 1] + INSERT_COST,
								  temp + DELETE_COST});
			}
			pre = temp;
		}
	}
	return dp[len2];
}

// BK-Tree 节点定义
struct BKNode {
    std::string word;
    std::map<int, std::shared_ptr<BKNode>> children;

    BKNode(const std::string &w) : word(w) {}
};

// BK-Tree 类定义
class BKTree {
public:
    BKTree() : root(nullptr) {}

    // 插入单词到 BK-Tree
    void insert(const std::string &word) {
        if (!root) {
            root = std::make_shared<BKNode>(word);
            return;
        }
        std::shared_ptr<BKNode> current = root;
        while (true) {
            int dist = minDistance(word, current->word);
            if (current->children.find(dist) == current->children.end()) {
                current->children[dist] = std::make_shared<BKNode>(word);
                break;
            }
            current = current->children[dist];
        }
    }

    // 搜索与给定单词编辑距离在阈值内的所有单词
    void search(const std::string &word, int threshold, std::vector<std::string> &results) {
        searchRecursive(root, word, threshold, results);
    }

private:
    std::shared_ptr<BKNode> root;

    void searchRecursive(std::shared_ptr<BKNode> node, const std::string &word, int threshold, std::vector<std::string> &results) {
        if (!node) return;

        int dist = minDistance(word, node->word);
        if (dist <= threshold) results.push_back(node->word);

        int start = dist - threshold;
        int end = dist + threshold;

        for (int i = start; i <= end; ++i) {
            if (node->children.find(i) != node->children.end()) {
                searchRecursive(node->children[i], word, threshold, results);
            }
        }
    }
};
int main() {
    BKTree tree;
    tree.insert("apple");
    tree.insert("apply");
    tree.insert("aple");
    tree.insert("app");
    // ... 其他单词

    std::vector<std::string> results;
    tree.search("apple", 2, results); // 搜索编辑距离在 2 以内的单词

    for (const auto &word : results) {
        std::cout << word << std::endl;
    }

    return 0;
}

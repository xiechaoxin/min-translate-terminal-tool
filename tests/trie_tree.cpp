/**
 * [208] 实现 Trie (前缀树)
 * Trie(发音类似 "try")或者说 前缀树 是一种树形数据结构,
 * 用于高效地存储和检索字符串数据集中的键. 这一数据结构有相当多的应用情景,
 * 例如自动补完和拼写检查.
 * 请你实现 Trie 类:
 *
 * Trie() 初始化前缀树对象.
 * void insert(String word) 向前缀树中插入字符串 word .
 * boolean search(String word) 如果字符串 word 在前缀树中, 返回
 * true(即, 在检索之前已经插入); 否则, 返回 false .
 * boolean startsWith(String prefix)
 * 如果之前已经插入的字符串 word 的前缀之一为 prefix , 返回 true ;
 * 否则, 返回 false .
 *
 * 示例:
 * 输入
 * ["Trie", "insert", "search", "search", "startsWith", "insert", "search"]
 * [[], ["apple"], ["apple"], ["app"], ["app"], ["app"], ["app"]]
 * 输出
 * [null, null, true, false, true, null, true]
 *
 * 解释
 * Trie trie = new Trie();
 * trie.insert("apple");
 * trie.search("apple");   // 返回 True
 * trie.search("app");     // 返回 False
 * trie.startsWith("app"); // 返回 True
 * trie.insert("app");
 * trie.search("app");     // 返回 True
 *
 * 提示:
 *
 * 1 <= word.length, prefix.length <= 2000
 * word 和 prefix 仅由小写英文字母组成
 * insert search 和 startsWith 调用次数 总计 不超过 3 * 104 次
 *
 * @lc app=leetcode.cn id=208 lang=cpp
 * https://leetcode.cn/problems/implement-trie-prefix-tree/
 * [208] Implement Trie (Prefix Tree)
 * Difficulty: medium
 * Tag: [hash-table, string, design, trie]
 *
 * A trie [https://en.wikipedia.org/wiki/Trie] (pronounced as "try") or prefix
 * tree is a tree data structure used to efficiently store and retrieve keys in
 * a dataset of strings. There are various applications of this data structure,
 * such as autocomplete and spellchecker.
 *
 * Implement the Trie class:
 *
 * Trie() Initializes the trie object.
 * void insert(String word) Inserts the string word into the trie.
 * boolean search(String word) Returns true if the string word is in the trie
 * (i.e., was inserted before), and false otherwise.
 * boolean startsWith(String prefix) Returns true if there is a previously
 * inserted string word that has the prefix prefix, and false otherwise.
 *
 * Example 1:
 * Input
 * ["Trie", "insert", "search", "search", "startsWith", "insert", "search"]
 * [[], ["apple"], ["apple"], ["app"], ["app"], ["app"], ["app"]]
 * Output
 * [null, null, true, false, true, null, true]
 *
 * Explanation
 * Trie trie = new Trie();
 * trie.insert("apple");
 * trie.search("apple");   // return True
 * trie.search("app");     // return False
 * trie.startsWith("app"); // return True
 * trie.insert("app");
 * trie.search("app");     // return True
 *
 * Constraints:
 * 1 <= word.length, prefix.length <= 2000
 * word and prefix consist only of lowercase English letters.
 * At most 3 * 104 calls in total will be made to insert, search, and
 * startsWith.
 */

#include <iostream>
#include <string>
#include <vector>

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

// const int ALPHABET_SIZE = 26;  // 假设只处理小写字母
const int ASCII_SIZE = 128;

struct TrieNode {
	bool isEnd;	 // 当前节点是否为一个单词的结尾
	std::vector<TrieNode *>
		children;  // 子节点指针数组, 用于存储所有可能的后继字符节点

	// 构造函数, 初始化 TrieNode
	TrieNode() : isEnd(false), children(ASCII_SIZE, nullptr) {}
};

class Trie {
private:
	TrieNode *root;	 // Trie 树的根节点

public:
	// 模糊搜索函数
	std::vector<std::string> fuzzySearch(const std::string &word, int limit) {
		std::vector<std::string> results;
		fuzzySearchHelper(root, word, "", limit, results);
		return results;
	}

public:
	Trie() {
		root = new TrieNode();
	}

	// 插入一个单词到 Trie 树中
	void insert(std::string word) {
		TrieNode *node = root;
		for (char c : word) {
			int idx = c - 'a';
			// 没有节点就新建
			if (!node->children[idx]) {
				node->children[idx] = new TrieNode();
			}
			node = node->children[idx];
		}
		node->isEnd = true;	 // 将最后一个字符的节点标记为单词结尾
	}

	// 判断一个单词是否在 Trie 树中
	bool search(const std::string &word) {
		TrieNode *node = root;
		for (char c : word) {
			int idx = c - 'a';
			if (!node->children[idx]) {
				return false;  // 不存在该单词的前缀, 直接返回 false
			}
			node = node->children[idx];
		}
		return node->isEnd;	 // 单词的最后一个字符节点是否为单词结尾
	}

	// 判断 Trie 树中是否有以给定前缀开头的单词
	bool startsWith(const std::string &prefix) {
		TrieNode *node = root;
		for (char c : prefix) {
			int idx = c - 'a';
			if (!node->children[idx]) {
				return false;  // 不存在该前缀, 直接返回 false
			}
			node = node->children[idx];
		}
		return true;  // 存在以给定前缀开头的单词
	}

	// 析构函数, 释放 Trie 树的内存
	~Trie() {
		deleteTrie(root);
	}

private:
	// 递归释放 Trie 树的内存
	void deleteTrie(TrieNode *node) {
		if (!node) {
			return;
		}
		for (TrieNode *child : node->children) {
			deleteTrie(child);
		}
		delete node;
	}

	void fuzzySearchHelper(TrieNode *node, const std::string &word,
						   const std::string &current, int limit,
						   std::vector<std::string> &results) {
		if (!node)
			return;
		if (node->isEnd && minDistance(current, word) <= limit) {
			results.push_back(current);
		}
		for (int i = 0; i < ASCII_SIZE; ++i) {
			if (node->children[i]) {
				char nextChar =
					static_cast<char>(i + 'a');	 // 假设只处理小写英文字母
				fuzzySearchHelper(node->children[i], word, current + nextChar,
								  limit, results);
			}
		}
	}
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */

int main(int argc, const char **argv) {
	// ["Trie","insert","search","search","startsWith","insert","search"]
	// [[],["apple"],["apple"],["app"],["app"],["app"],["app"]]
	Trie trie;
	trie.insert("apple");
	trie.insert("apply");
	trie.insert("app");
	trie.insert("apphahshdfa");
	trie.insert("appiads");
	trie.insert("appthis");
	// ...

	std::vector<std::string> matches =
		trie.fuzzySearch("apl", 3);	 // 查找与 "apl" 编辑距离在 1 以内的所有单词
	for (const std::string &match : matches) {
		std::cout << match << std::endl;
	}
	return 0;
}

#include "trie_tree.h"
#include "search.h"
#include "config.h"

void Trie::insert(std::string word) {
	TrieNode *node = root;
	for (char c : word) {
		// 没有节点就新建
		if (!node->children[c]) {
			node->children[c] = new TrieNode();
		}
		node = node->children[c];
	}
	node->isEnd = true;	 // 将最后一个字符的节点标记为单词结尾
}

bool Trie::search(const std::string &word) {
	TrieNode *node = root;
	for (char c : word) {
		if (!node->children[c]) {
			return false;  // 不存在该单词的前缀, 直接返回 false
		}
		node = node->children[c];
	}
	return node->isEnd;	 // 单词的最后一个字符节点是否为单词结尾
}

bool Trie::startsWith(const std::string &prefix) {
	TrieNode *node = root;
	for (char c : prefix) {
		if (!node->children[c]) {
			return false;  // 不存在该前缀, 直接返回 false
		}
		node = node->children[c];
	}
	return true;  // 存在以给定前缀开头的单词
}

Trie::Trie() {
	root = new TrieNode();
}
Trie::~Trie() {
	deleteTrie(root);
}

void Trie::deleteTrie(TrieNode *node) {
	if (!node) {
		return;
	}
	for (TrieNode *child : node->children) {
		deleteTrie(child);
	}
	delete node;
}

std::vector<std::string> Trie::fuzzySearch(const std::string &word) {
	std::vector<std::string> results;
	TrieNode *current = root;

	// 首先查找 word 是否存在
	for (char ch : word) {
		if (!current->children[ch]) {
			return results;	 // 如果 word 不存在，返回空结果
		}
		current = current->children[ch];
	}

	// 从 word 的最后一个字符的 TrieNode 开始，查找最多10个单词
	fuzzySearchHelper(current, word, word, results);
	return results;
}

// 模糊搜索帮助函数
void Trie::fuzzySearchHelper(TrieNode *node, const std::string &word,
							 std::string current,
							 std::vector<std::string> &results) {
	if (results.size() >= CANDIDATES_NUMBER) {
		return;
	}

	if (node->isEnd) {
		results.push_back(current);	 // 找到一个完整的单词
	}

	for (int i = 0; i < ASCII_SIZE; i++) {
		if (node->children[i] != nullptr) {
			// 继续在子节点中搜索
			char nextChar = i;
			fuzzySearchHelper(node->children[i], word, current + nextChar,
							  results);
		}
	}
}

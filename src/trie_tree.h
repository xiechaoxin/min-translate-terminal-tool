

#ifndef _TRIE_TREE_H_
#define _TRIE_TREE_H_

#pragma once

#include "utils.h"

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
	std::vector<std::string> fuzzySearch(const std::string &word);

public:
	Trie();

	// 插入一个单词到 Trie 树中
	void insert(std::string word);

	// 判断一个单词是否在 Trie 树中
	bool search(const std::string &word);

	// 判断 Trie 树中是否有以给定前缀开头的单词
	bool startsWith(const std::string &prefix);

	// 析构函数, 释放 Trie 树的内存
	~Trie();

private:
	// 递归释放 Trie 树的内存
	void deleteTrie(TrieNode *node);

	void fuzzySearchHelper(TrieNode *node, const std::string &word,
								 std::string current, std::vector<std::string> &results);
};

#endif /* _TRIE_TREE_H_ */

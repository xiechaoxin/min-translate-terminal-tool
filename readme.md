# 简介

这是一个基于终端的简易翻译工具。

## 功能

- 翻译(中英)
- 模糊搜索 (fuzzy search)
- 翻译历史 (todo: 现在有log，加入查看翻译历史, 感觉没啥必要(2023-12-22))
- 翻译词频 (todo: 这个有必要，优化log之后，从log中分析)

# 关键数据结构

## 字典文件格式

```txt
{en}\t{zh}
{en}\t{zh}
{en}\t{zh}
```

## 字典内存格式

### en->zh

en_key: 去除了标点, 可以设置大小写敏感 config.h::CASE_SENSITIVE;

trie-tree: use en_key as node;

u_map<en_key:str, struct{en_src:str, zh_src:str}> dictionary;

### zh->en

zh_key: 去除了标点; 中文分词时候，少于一个字，我直接删除了，所以如果搜索中文(一个字)会查无结果, 这是故意地，不是bug

u_map<en_key:str, vec<struct{en_key:str, weight:int>> InvertedIndex;

# 问题或改进：

在C++中使用utf8字符串是一个问题，我自己写了一个类(utf8string), 但是最终还是使用了Python进行分词，直接序列化到文件，用C++读取文件，这样减低了C++的中文字符处理难度，不用运行时分词，也使得程序变快了。

# 使用到的字符串算法

1. inverted-index 倒排索引
2. trie-tree 字典树

# 可能的提升方向

1. **Levenshtein Distance (编辑距离)**: 这是一种衡量两个字符串差异的方法。它计算将一个字符串转换成另一个字符串所需的最少单字符编辑（插入、删除或替换）的数量。
2. **Trie (字典树)**: Trie 树是一种用于存储字符串的树形数据结构，可以高效地检索字符串数据集中的键。结合编辑距离，Trie 可以用于实现高效的模糊搜索。
3. **BK-Trees (Burkhard-Keller Trees)**: BK-Tree 是一种基于度量空间的树形数据结构，适用于编辑距离等度量。它可以用于快速查询与给定字符串在一定编辑距离内的所有字符串。
4. **SimHash 或局部敏感哈希**：这些技术用于快速近似相似性检测，适用于大规模数据。
5. **multi-thread**: 为了在多线程环境中实现模糊搜索，您可以将数据分割成多个部分，每个线程处理一部分数据。例如，如果您使用 Trie 树，可以为树的每个主要分支分配一个线程。

# 使用到的第三方库

~~1. jieba: 分词~~

# log
[2023-12-22]改成使用thulac分词, 重新生成inverted-index.txt

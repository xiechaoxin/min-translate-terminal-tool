#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#pragma once

#include <unordered_map>
#include <fstream>
#include <mutex>
#include <sstream>
#include "utils.h"
#include "trie_tree.h"
#include <string>
#include <iostream>

struct en_zh {
	std::string en;
	std::string zh;
	en_zh() = default;
	en_zh(std::string en, std::string zh) : en(en), zh(zh) {}
};

extern std::unordered_map<std::string, en_zh> dictionary;

Trie* init_db();

#endif /* _DICTIONARY_H_ */

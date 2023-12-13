#ifndef _UTILS_H_
#define _UTILS_H_

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

namespace utils {

bool is_ascii(const std::string &word);

std::string removePunctuation(const std::string &word);

std::string to_lowers(std::string &word);

template <typename T>
void printVector(const std::vector<T> &vec);

void printDictionary(const std::string& key);
void printDictionary(std::vector<std::string> vector1);
}  // namespace utils
#endif /* _UTILS_H_ */

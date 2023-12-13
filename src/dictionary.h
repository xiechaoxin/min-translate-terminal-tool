#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#pragma once

#include <unordered_map>
#include <fstream>
#include <mutex>
#include <sstream>
#include "utils.h"
#include <string>
#include <iostream>

extern std::unordered_map<std::string, std::string> dictionary;

void init_db();

#endif /* _DICTIONARY_H_ */

//
// Created by xcx on 2023/12/15.
//
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../../include/utf8.h"

using namespace std;

bool valid_utf8_file(const char *file_name) {
	ifstream ifs(file_name);
	if (!ifs)
		return false;  // even better, throw here

	istreambuf_iterator<char> it(ifs.rdbuf());
	istreambuf_iterator<char> eos;

	return utf8::is_valid(it, eos);
}
template <typename octet_iterator>
uint32_t next(octet_iterator& it, octet_iterator end);

int main(int argc, char **argv) {
	char utf_invalid[] = "nihk妳好吗";
	bool bvalid = utf8::is_valid(utf_invalid, utf_invalid + 6);
	assert(bvalid == false);
	std::string str = u8"中文";
	auto it = str.begin();

	while (it != str.end()) {
		uint32_t cp = utf8::next(it, str.end());  // Decodes the next UTF-8 code point and advances the iterator
		std::cout << "Code point: " << cp << std::endl;
	}
	string st = u8"中文";
	std::cout << st.size() << std::endl;

	return 0;
}
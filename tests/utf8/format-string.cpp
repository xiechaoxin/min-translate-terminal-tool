//
// Created by xcx on 2023/12/16.
//



#include <iostream>
#include <string>
#include <sstream>

class FormatString {
public:
	template <typename T>
	FormatString &operator<<(const T &value) {
		oss << value;
		return *this;
	}

	std::string str() const {
		return oss.str();
	}

private:
	std::ostringstream oss;
};

template <typename... Args>
std::string format(Args... args) {
	FormatString fs;
	(fs << ... << args);  // 折叠表达式
	return fs.str();
}

int main(int argc, const char **argv) {
	int a = 10;
	std::string b = "hell中";

	std::string result = format(a, "---lalal中文", b);
	std::cout << result << std::endl;  // 输出 "10---lalal中文hell中"
	return 0;
}
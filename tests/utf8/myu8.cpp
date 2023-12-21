//
// Created by xcx on 2023/12/15.
//

#include "utf8string.h"

using namespace std;

int main() {
	std::string str = "中aHello, 世界bcd文";  // UTF-8 字符串
	// Utf8String utf8str(str);
	// std::cout << utf8str.size() << std::endl;
	// for (const auto &item : utf8str) {
	// 	std::cout << item << std::endl;
	// }
	// string s;			//  默认初始化，一个空白的字符串
	// string s1("ssss");	// s1是字面值"ssss"的副本
	//
	// Utf8String utf8str1(s1);
	// string s2(s1);	// s2是s1的副本
	// Utf8String utf8str2(s2);
	// string s3 = s2;	 // s3是s2的副本
	// Utf8String utf8str3(s3);
	// string s4(10, '4');	 // s4初始化
	// Utf8String utf8str4(s4);
	// string s5 = "Andre";  // 拷贝初始化
	// Utf8String utf8str5(s5);
	// string s6 =
	// 	string(10, 'c');  // 可拷贝初始化，生成一个初始化好的对象，拷贝给s6
	// Utf8String utf8str6(s6);
	//
	// char cs[] = "12345";
	// string s7(cs, 3);  // 复制字符串cs的前三个字符到s当中
	//
	// Utf8String utf8str7(s7);
	// string s8 = "abcde";
	// Utf8String utf8str8(s8);
	// string s9(s8, 2);
	// Utf8String utf8str9(s9);
	//
	// string s10 = "asdsfasdgf";
	// Utf8String utf8strs10(s10);
	// string s11(s10, 3,
	// 		   4);	// s4是s3从下标s开始4个字符的拷贝，超出s10.size出现未定义
	// Utf8String utf8str11(s11);
	// string sss = utf8str11;
	// std::cout << sss << std::endl;

	// std::string str1, str2;
	//
	// std::cout << "Enter a string (std::cin): ";
	// std::cin >> str1;
	// std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略剩余的字符，直到换行符
	//
	// std::cout << "Enter another string (getline): ";
	// std::getline(std::cin, str2);
	//
	// std::cout << "String from std::cin: " << str1 << std::endl;
	// std::cout << "String from getline: " << str2 << std::endl;

	// std::cout << "final" << std::endl;
	// string s122 = "10";
	// s.append("hds");
	// std::cout << s << std::endl;
	// Utf8String utf8str("Hello, ");
	// utf8str += "世";		// 添加单个字符
	// utf8str.append("界!");	// 添加字符串

	// std::cout << "Utf8String content: " << utf8str << std::endl;
	// for (auto &&it : utf8str) {
	// 	std::cout << it << std::endl;
	// }

	// 使用 std::stringstream 来读取数据
	// std::stringstream ss("Hello, 世界!");
	// Utf8String ur;
	// printf("---------------------");
	// while (ss >> ur) {
	// 	std::cout << ur << std::endl;
	//
	// }

	// std::cout << ur << std::endl;
	// Utf8StringStream ss1;
	// ss1 << "en"
	// 	<< "works中文vw";
	// std::cout << ss1.str() << std::endl;
	// std::cout << ss1.str().size() << std::endl;

	// int a = 10;
	// std::string b = "hell中";

	// std::ostringstream oss;
	// oss << a << utf8str << "---lalal中文" << b;

	// std::string result = oss.str();
	// std::cout << result << std::endl;  // 输出 "10---lalal中文hell中"


	// std::cout << "---------------------" << std::endl;
	// {
	// 	int a = 10;
	// 	std::string str = "world";
	//
	// 	std::string result = fmt::format("a:{},\\{\\}\\{\\};str:{}, djshi中文", a, str);
	//
	// 	std::cout << result << std::endl;
	// 	std::cout << result.size() << std::endl;
	//
	// 	std::cout << "-=----------------------------------------\n";
	// 	string sss;
	// 	sss = fmt::format("a{}, str{}", a, str);
	// 	std::cout << sss << std::endl;
	//
	//
	// 	auto result2 = Utf8String::format("a:{},\\{\\}\\{\\};str:{}, djshi中文", a, str);
	// 	std::cout << result2 << std::endl;
	// 	std::cout << result2.size() << std::endl;
	//
	// 	sss.substr();
	// 	result2.append("中文");
	//
	// 	auto res = result2 + "中文";
	//
	// 	std::cout << res << std::endl;
	// 	std::cout << res.size() << std::endl;
	// 	char cs[] = "12345";
	// 	Utf8String s7(cs, 3);
	// 	std::cout << s7 << std::endl;
	//
	// 	// cin >> s7;
	//
	// 	// getline(cin,  s7);
	// 	// std::cout << s7.size() << std::endl;
	// 	// Utf8String utf8str("abcdefg中文测试");
	// 	// Utf8String subu8str("中文");
	// 	// int pos = utf8str.find(subu8str);
	// 	// std::cout << "Position: " << pos << std::endl;  // 输出子字符串的位置
	// 	//
	// 	//
	// 	// Utf8String s("abcdefg中文测试");
	//
	// 	Utf8String utf8str("abcdefga中文a测试");
	// 	Utf8String subStr = utf8str.substr(7, 2);
	// 	std::cout << "SubString: " << subStr << std::endl;  // 输出子字符串的内容
	// 	std::cout << "SubString: " << subStr.size() << std::endl;  // 输出子字符串的内容
	// 	std::cout << "----------------------------------------------------------------------" << std::endl;
	// 	Utf8String utf8str2("abcdefg中文测试");
	// 	auto it = utf8str2.begin() + 7; // 定位到 "中"
	// 	utf8str2.erase(it); // 删除 "中"
	// 	std::cout << utf8str2 << std::endl; // 应该输出 "abcdefg文测试"
	//
	// 	std::cout << "----------------------------------------------------------------------" << std::endl;
	// 	Utf8String utf8str3("abcdefg中文测试");
	// 	utf8str3.erase(utf8str3.begin() + 7, utf8str3.begin() + 9); // 删除 "中文"
	// 	std::cout << utf8str3 << std::endl; // 应该输出 "abcdefg测试"
	// 	string ssss;
	// 	// ssss.find();
	//
	// }

	{
		// Utf8String u8str("");
		string u8str = "test";
		std::string s = fmt::format("Number: {{{}}}, Esc{{}}aped braces: {}", 42, u8str);
		std::cout << s << std::endl;
		Utf8String u8s;
	}


	return 0;
}

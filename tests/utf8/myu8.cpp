//
// Created by xcx on 2023/12/15.
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iterator>

namespace fmt {

// 将参数转换为字符串
template <typename T>
std::string toString(const T &value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

// 递归终止函数
void process(std::ostringstream &oss, const std::string &fmt, size_t &pos) {
	if (pos < fmt.length()) {
		oss << fmt.substr(pos);
	}
}

// 递归解包参数并替换占位符
template <typename T, typename... Args>
void process(std::ostringstream &oss, const std::string &fmt, size_t &pos,
			 const T &value, Args... args) {
	size_t start = fmt.find("{}", pos);
	if (start == std::string::npos) {
		oss << fmt.substr(pos);
		return;
	}
	oss << fmt.substr(pos, start - pos) << toString(value);
	pos = start + 2;  // 跳过 "{}"
	process(oss, fmt, pos, args...);
}

// 主格式化函数
template <typename... Args>
std::string format(const std::string &fmt, Args... args) {
	std::ostringstream oss;
	size_t pos = 0;
	process(oss, fmt, pos, args...);
	return oss.str();
}
}  // namespace fmt

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

class Utf8String {
public:
	Utf8String() = default;

	// 主构造函数，使用右值引用以支持移动语义
	Utf8String(std::string &&s) : str(std::move(s)) {
		buildIndices();
	}

	// 重载构造函数，直接接受 std::string 对象（避免不必要的转换）
	Utf8String(const std::string &s) : str(s) {
		buildIndices();
	}

	// 允许任何可以转换为 std::string 的类型作为构造参数
	template <typename... Args>
	Utf8String(Args &&...args)
		: Utf8String(std::string(std::forward<Args>(args)...)) {}

	std::string getStr() const {
		return str;
	}

	// 提供索引访问（类似于 std::string 的 operator[]）
	std::string operator[](size_t n) const {
		return getUtf8Char(n);
	}

	// 提供边界检查的索引访问（类似于 std::string 的 at()）
	std::string at(size_t n) const {
		if (n >= utf8CharIndices.size()) {
			throw std::out_of_range("Index out of range");
		}
		return getUtf8Char(n);
	}

	size_t size() const {
		return utf8CharIndices.size();
	}

	bool empty() const {
		return str.empty();
	}

	void clear() {
		str.clear();
		utf8CharIndices.clear();
	}

	/*
TODO<2023-12-16, @xcx>
局部更新索引：当向字符串添加内容时，只更新影响到的索引部分。例如，如果在字符串末尾添加内容，您只需要添加新字符的索引，而不需要重新处理整个字符串。

惰性索引构建：只在实际需要索引（如进行随机访问操作）时构建或更新索引，而不是在每次字符串修改时都更新。这可以通过标记索引为“脏”（即需要更新）来实现，然后在需要索引时进行构建。

优化存储结构：如果 Utf8String 类主要用于追加操作，可以考虑使用更适合此类操作的数据结构，如链表，来存储字符索引。这样，追加操作将只涉及到链表的尾部，而不需要重新构建整个索引。

批量处理更新：如果您的应用程序允许，可以收集多个修改操作，并在必要时一次性地更新索引。这种方法可以减少索引重建的次数。
*/
	/* TODO<2023-12-16, @xcx>
	 * insert()
	 * 简单粗暴，每次都更新全部索引。。。
	 * 更新索引不好搞，
	 * 是否采用惰性更新(等调用到下次使用str再更新索引? )
	 * */

	/* TODO<2023-12-16, @xcx>
	 * erase()
	 * erase(pos, n);      //删除从pos开始的n个字符,比如erase(0, 1)就是删除第一个字符
	 * erase(position);    //删除从position处的一个字符(position是个string类型的迭代器)
	 * erase(first, last); //删除从first到last之间的字符(first和last都是迭代器)
	 * 同样更新索引不好搞
	 * */

	/* TODO<2023-12-16, @xcx>
	 * substr()
	 * string substr(size_t pos = 0, size_t len = npos) const;
	 * */

	// 添加字符串
	Utf8String &append(char ch) {
		str += ch;
		buildIndices();
		return *this;
	}

	// 添加字符串
	Utf8String &append(const std::string &s) {
		str.append(s);
		buildIndices();	 // 重新构建索引
		return *this;
	}

	// 添加字符串
	Utf8String &operator+=(const std::string &s) {
		str.append(s);
		buildIndices();
		return *this;
	}

	// 添加单个字符
	Utf8String &operator+=(char ch) {
		str += ch;
		buildIndices();	 // 重新构建索引
		return *this;
	}

	// cin >> utf8str; //不能读入空格，以空格，制表符，回车符作为结束标志
	friend std::istream &operator>>(std::istream &is, Utf8String &utf8str) {
		std::string temp;
		is >> temp;
		/* TODO<2023-12-16, @xcx> 每次都会重建索引 */
		utf8str = Utf8String(temp);
		return is;
	}

	// cout << utf8str;
	friend std::ostream &operator<<(std::ostream &os,
									const Utf8String &utf8str) {
		os << utf8str.str;
		return os;
	}

	// getline(cin, utf8str); //可以读入空格和制表符，以回车符作为结束的标志
	friend std::istream &getline(std::istream &is, Utf8String &utf8str) {
		std::string temp;
		std::getline(is, temp);
		utf8str = Utf8String(temp);
		return is;
	}

	// 提供隐式转换方法将 Utf8String 转换为 std::string，方便输出
	// Utf8String utf8str("Hello, 世界!");
	// std::cout << utf8str;
	// std::string stdStr = utf8str;
	operator std::string() const {
		return str;
	}

	// 类实现迭代器。这样的实现会让你能够遍历每个 UTF-8 编码的字符，就像遍历一个常规的字符数组一样。
	class iterator
		: public std::iterator<std::input_iterator_tag, std::string> {
	public:
		iterator(const Utf8String *u8str, size_t pos)
			: u8str(u8str), pos(pos) {}

		iterator &operator++() {
			++pos;
			return *this;
		}

		iterator operator++(int) {
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator==(const iterator &other) const {
			return pos == other.pos;
		}

		bool operator!=(const iterator &other) const {
			return pos != other.pos;
		}

		std::string operator*() const {
			return (*u8str)[pos];
		}

	private:
		const Utf8String *u8str;
		size_t pos;
	};

	iterator begin() const {
		return iterator(this, 0);
	}

	iterator end() const {
		return iterator(this, size());
	}

private:
	std::string str;					  // 原始字符串
	std::vector<size_t> utf8CharIndices;  // 记录每个u8字符的起始位置

private:
	// 辅助函数，用于获取第 n 个 UTF-8 字符
	// warning: 可能不适用于所有 UTF-8 编码的复杂性(例如，它不处理 Unicode 的合成字符)
	std::string getUtf8Char(size_t n) const {
		size_t start = utf8CharIndices[n];
		size_t len = (n + 1 < utf8CharIndices.size())
						 ? utf8CharIndices[n + 1] - start
						 : str.size() - start;
		return str.substr(start, len);
	}

	// 构建 UTF-8 字符的索引
	void buildIndices() {
		utf8CharIndices.clear();
		/// 为了使 Utf8String 类的 operator[] 具有 O(1) 的时间复杂度，我们可以在类初始化时预计算并存储每个 UTF-8 字符的起始位置。这可以通过使用一个 std::vector 来完成，该 vector 将保存字符串中每个 UTF-8 字符的起始索引。
		size_t i = 0;
		while (i < str.size()) {
			utf8CharIndices.push_back(i);
			i += getUtf8CharLength(str[i]);
		}
	}

	/// 判断一个字节是否是UTF-8的开始字节
	///
	/// 这行代码是一个布尔表达式，用于检测一个字节 `c` 是否是 UTF-8 编码字符序列中的起始字节。它是 UTF-8 字符串处理中的关键部分，用于正确地识别和处理多字节字符。这里是它的工作原理的详细解释：
	///
	/// 1. **UTF-8 编码原理**：UTF-8 是一种可变长度的字符编码，用于编码 Unicode 字符。它将每个 Unicode 字符编码为 1 到 4 个字节。UTF-8 字符串中的每个字节都可以分为两类：**起始字节**和**连续字节**。起始字节用于表示一个新字符的开始，而连续字节是跟随在起始字节后的字节，用于表示更多的字符信息。
	///
	/// 2. **位运算**：
	///    - `c & 0xC0`：这是一个按位与操作。`0xC0` 的二进制表示是 `11000000`。这个操作的目的是保留 `c` 的最高两位，而将其他位清零。
	///    - 比如，如果 `c` 是 `11010101`（一个典型的 UTF-8 起始字节），那么 `c & 0xC0` 的结果将是 `11000000`。
	///
	/// 3. **判断是否为起始字节**：
	///    - 在 UTF-8 编码中，连续字节的最高两位总是 `10`，即二进制的 `10000000`（或十六进制的 `0x80`）。
	///    - 因此，如果 `c & 0xC0` 的结果不等于 `0x80`，那么 `c` 不是连续字节，它是一个起始字节。
	///    - 如果 `c & 0xC0` 等于 `0x80`，那么 `c` 是一个连续字节，因为它的最高两位是 `10`。
	///
	/// 所以，这行代码 `return (c & 0xC0) != 0x80;` 是在检查 `c` 是否不是一个连续字节，从而判断它是否是一个 UTF-8 起始字节。如果是，这意味着 `c` 表示一个新的 UTF-8 字符的开始。
	bool isUtf8StartByte(unsigned char c) const {
		return (c & 0xC0) != 0x80;
	}

	/// 每个 UTF-8 字符的第一个字节都有特定的位模式，这使我们能够区分它和多字节序列中的后续字节。
	/// 如果最高一位为 0 ((c & 0x80) == 0)，则这是一个单字节字符（标准 ASCII）。
	/// 如果最高两位为 110 ((c & 0xE0) == 0xC0)，则这是一个双字节字符。
	/// 如果最高三位为 1110 ((c & 0xF0) == 0xE0)，则这是一个三字节字符。
	/// 如果最高四位为 11110 ((c & 0xF8) == 0xF0)，则这是一个四字节字符。
	size_t getUtf8CharLength(unsigned char c) const {
		if ((c & 0x80) == 0)
			return 1;
		if ((c & 0xE0) == 0xC0)
			return 2;
		if ((c & 0xF0) == 0xE0)
			return 3;
		if ((c & 0xF8) == 0xF0)
			return 4;
		return 1;  // Should not reach here if valid UTF-8
	}
};

class Utf8StringStream {
public:
	Utf8StringStream &operator<<(const std::string &s) {
		utf8Str += s;
		return *this;
	}

	Utf8String str() const {
		return utf8Str;
	}

private:
	Utf8String utf8Str;
};

int main() {
	std::string str = "中aHello, 世界bcd文";  // UTF-8 字符串
	// Utf8String utf8str(str);
	// std::cout << utf8str.size() << std::endl;
	// for (const auto &item : utf8str) {
	// 	std::cout << item << std::endl;
	// }
	using namespace std;
	string s;			//  默认初始化，一个空白的字符串
	string s1("ssss");	// s1是字面值"ssss"的副本

	Utf8String utf8str1(s1);
	string s2(s1);	// s2是s1的副本
	Utf8String utf8str2(s2);
	string s3 = s2;	 // s3是s2的副本
	Utf8String utf8str3(s3);
	string s4(10, '4');	 // s4初始化
	Utf8String utf8str4(s4);
	string s5 = "Andre";  // 拷贝初始化
	Utf8String utf8str5(s5);
	string s6 =
		string(10, 'c');  // 可拷贝初始化，生成一个初始化好的对象，拷贝给s6
	Utf8String utf8str6(s6);

	char cs[] = "12345";
	string s7(cs, 3);  // 复制字符串cs的前三个字符到s当中

	Utf8String utf8str7(s7);
	string s8 = "abcde";
	Utf8String utf8str8(s8);
	string s9(s8, 2);
	Utf8String utf8str9(s9);

	string s10 = "asdsfasdgf";
	Utf8String utf8strs10(s10);
	string s11(s10, 3,
			   4);	// s4是s3从下标s开始4个字符的拷贝，超出s10.size出现未定义
	Utf8String utf8str11(s11);
	string sss = utf8str11;
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

	std::cout << "final" << std::endl;
	string s122 = "10";
	s.append("hds");
	std::cout << s << std::endl;
	Utf8String utf8str("Hello, ");
	utf8str += "世";		// 添加单个字符
	utf8str.append("界!");	// 添加字符串

	// std::cout << "Utf8String content: " << utf8str << std::endl;
	// for (auto &&it : utf8str) {
	// 	std::cout << it << std::endl;
	// }

	// 使用 std::stringstream 来读取数据
	std::stringstream ss("Hello, 世界!");
	Utf8String ur;
	printf("---------------------");
	// while (ss >> ur) {
	// 	std::cout << ur << std::endl;
	//
	// }

	// std::cout << ur << std::endl;
	Utf8StringStream ss1;
	ss1 << "en"
		<< "works中文vw";
	std::cout << ss1.str() << std::endl;
	std::cout << ss1.str().size() << std::endl;

	// int a = 10;
	// std::string b = "hell中";

	// std::ostringstream oss;
	// oss << a << utf8str << "---lalal中文" << b;

	// std::string result = oss.str();
	// std::cout << result << std::endl;  // 输出 "10---lalal中文hell中"

	int a = 10;
	std::string b = "hell中";

	std::string result = format(a, "---lalal中文", b);
	std::cout << result << std::endl;  // 输出 "10---lalal中文hell中"

	{
		int a = 10;
		std::string str = "world";

		std::string result = fmt::format("a:{},{} str:{}, djshi中文", a, str);
		std::cout << result << std::endl;
	}

	return 0;
}

/* TODO<2023-12-16, @xcx> f'{}'
 * f"%d" % 10
 * 类似格式化字符串的方法，format
 */
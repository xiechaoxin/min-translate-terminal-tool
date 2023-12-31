
#ifndef TRANSLATEAPP_UTF8STRING_H
#define TRANSLATEAPP_UTF8STRING_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <iterator>

class fmt {
public:
	// 主格式化函数（静态）
	template <typename... Args>
	static std::string format(const std::string &fmt, Args... args) {
		std::ostringstream oss;
		size_t pos = 0;
		try {
			process(oss, fmt, pos, args...);

		} catch (const std::exception &e) {
			// 异常处理逻辑
			std::cerr << "Exception occurred in format: " << e.what() << std::endl;
			throw;
		}
		std::string res = oss.str();
		replace(res, "{{", "{");
		replace(res, "}}", "}");
		return res;
	}

private:
	static void replace(std::string &str, const std::string &from, const std::string &to) {
		size_t pos = 0;
		while ((pos = str.find(from, pos)) != std::string::npos) {
			str.replace(pos, from.length(), to);
			pos += to.length();	 // 移动到此次替换的后面，避免无限循环
		}
	}

	// 将参数转换为字符串
	template <typename T>
	static std::string toString(const T &value) {
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	// 寻找下一个未转义的占位符位置
	static size_t findNextPlaceholder(const std::string &fmt, size_t pos) {
		while (pos < fmt.length()) {
			if (fmt[pos] == '{') {
				if (pos + 1 < fmt.length() && fmt[pos + 1] == '{') {
					pos += 2;  // 跳过转义的 '{{'
				} else {
					return pos;	 // 找到未转义的 '{'
				}
			} else if (fmt[pos] == '}') {
				if (pos + 1 < fmt.length() && fmt[pos + 1] == '}') {
					pos += 2;  // 跳过转义的 '}}'
				} else {
					throw std::runtime_error("Unmatched '}' in format string");
				}
			} else {
				++pos;	// 继续查找
			}
		}
		return std::string::npos;  // 未找到占位符
	}

	// 递归终止函数
	static void process(std::ostringstream &oss, const std::string &fmt, size_t &pos) {
		if (pos < fmt.length()) {
			oss << fmt.substr(pos);
		}
	}

	// 递归解包参数并替换占位符
	template <typename T, typename... Args>
	static void process(std::ostringstream &oss, const std::string &fmt, size_t &pos, const T &value, Args... args) {
		while (pos < fmt.length()) {
			size_t start = findNextPlaceholder(fmt, pos);

			// 处理普通文本（包括转义的花括号）
			oss << fmt.substr(pos, start - pos);

			if (start == std::string::npos) {
				break;	// 没有更多的占位符
			}

			// 处理占位符
			size_t end = fmt.find('}', start);
			if (end == std::string::npos) {
				throw std::runtime_error("Unmatched '{' in format string");
			}
			oss << toString(value);
			pos = end + 1;
			process(oss, fmt, pos, args...);
			return;
		}
	}

	// // 将递归，使用迭代实现
	// template <typename... Args>
	// static void process(std::ostringstream &oss, const std::string &fmt, size_t &pos, Args... args) {
	// 	std::array<std::function<void(std::ostringstream&)>, sizeof...(Args)> argFunctions = {
	// 		[&args](std::ostringstream &oss){ oss << args; }...
	// 	};
	//
	// 	size_t argIndex = 0;
	//
	// 	while (pos < fmt.length()) {
	// 		size_t start = findNextPlaceholder(fmt, pos);
	//
	// 		// 处理普通文本（包括转义的花括号）
	// 		oss << fmt.substr(pos, start - pos);
	//
	// 		if (start == std::string::npos) {
	// 			break; // 没有更多的占位符
	// 		}
	//
	// 		// 处理占位符
	// 		size_t end = fmt.find('}', start);
	// 		if (end == std::string::npos) {
	// 			throw std::runtime_error("Unmatched '{' in format string");
	// 		}
	// 		if (argIndex >= argFunctions.size()) {
	// 			throw std::runtime_error("More placeholders than arguments");
	// 		}
	//
	// 		argFunctions[argIndex++](oss);
	// 		pos = end + 1;
	// 	}
	//
	// 	// 确保所有参数都已使用
	// 	if (argIndex < sizeof...(args)) {
	// 		throw std::runtime_error("More arguments than placeholders");
	// 	}
	// }
};

class Utf8String {
public:
	Utf8String() : dirty(true) {}

	// 主构造函数，使用右值引用以支持移动语义
	Utf8String(std::string &&s) : str(std::move(s)), dirty(true) {}

	// 重载构造函数，直接接受 std::string 对象（避免不必要的转换）
	Utf8String(const std::string &s) : str(s), dirty(true) {}

	// 允许任何可以转换为 std::string 的类型作为构造参数
	template <typename... Args>
	Utf8String(Args &&...args) : Utf8String(std::string(std::forward<Args>(args)...)) {}

	// 添加 find 方法
	// 返回子串在原字符串中的u8字符索引
	int find(const std::string &subs) const {
		ensureIndexBuilt();
		size_t bytePos = str.find(subs);
		if (bytePos == std::string::npos) {
			return -1;	// 没有找到子字符串
		}
		// 使用 utf8CharIndices 找到字符位置
		return getCharPosition(bytePos);
	}

	// 添加 substr 方法
	Utf8String substr(size_t charPos = 0, size_t charLen = std::string::npos) const {
		ensureIndexBuilt();
		if (charPos >= utf8CharIndices.size()) {
			return Utf8String("");	// 超出范围
		}

		size_t byteStart = utf8CharIndices[charPos];
		size_t byteEnd;

		if (charLen == std::string::npos || charPos + charLen >= utf8CharIndices.size()) {
			byteEnd = str.length();	 // 截取到字符串末尾
		} else {
			byteEnd = utf8CharIndices[charPos + charLen];
		}

		return Utf8String(str.substr(byteStart, byteEnd - byteStart));
	}

	// 主格式化函数（静态）
	template <typename... Args>
	static Utf8String format(const std::string &fmt, Args... args) {
		return fmt::format(fmt, args...);
	}

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
		return getUtf8Char(n);	// include ensureIndexBuilt()
	}

	size_t size() const {
		ensureIndexBuilt();
		return utf8CharIndices.size();
	}

	bool empty() const {
		return str.empty();
	}

	void clear() {
		str.clear();
		dirty = true;
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

	Utf8String &operator+(const char ch) {
		str += ch;
		dirty = true;
		return *this;
	}

	Utf8String &operator+(const std::string &s) {
		str.append(s);
		dirty = true;
		return *this;
	}

	// 添加字符串
	Utf8String &operator+=(const std::string &s) {
		str.append(s);
		dirty = true;
		return *this;
	}

	// 添加单个字符
	Utf8String &operator+=(char ch) {
		str += ch;
		dirty = true;
		return *this;
	}

	// cin >> utf8str; //不能读入空格，以空格，制表符，回车符作为结束标志
	friend std::istream &operator>>(std::istream &is, Utf8String &utf8str) {
		std::string temp;
		is >> temp;
		utf8str = Utf8String(temp);
		return is;
	}

	// cout << utf8str;
	friend std::ostream &operator<<(std::ostream &os, const Utf8String &utf8str) {
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
	class iterator : public std::iterator<std::input_iterator_tag, std::string> {
	public:
		iterator(Utf8String *u8str, size_t pos) : u8str(u8str), pos(pos) {}

		// 获取底层 std::string 的迭代器
		std::string::iterator getBaseIterator() const {
			size_t bytePos = u8str->getBytePosition(pos);
			return u8str->str.begin() + bytePos;
		}

		// 重载运算符以支持与整数的加法
		iterator operator+(int n) const {
			return iterator(u8str, pos + n);
		}

		// 重载运算符以支持与整数的减法
		iterator operator-(int n) const {
			return iterator(u8str, pos - n);
		}

		iterator &operator++() {
			++pos;
			return *this;
		}

		iterator operator++(int) {
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

		// 前缀递减运算符
		iterator &operator--() {
			if (pos > 0) {
				--pos;
			}
			return *this;
		}

		// 后缀递减运算符
		iterator operator--(int) {
			iterator tmp = *this;
			if (pos > 0) {
				--pos;
			}
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

		// 获取当前迭代器的字符位置
		size_t getCharPosition() {
			return pos;	 // 返回迭代器当前的u8字符位置
		}

	private:
		Utf8String *u8str;
		size_t pos;	 // 当前迭代器的u8字符位置
	};

	iterator begin() {
		ensureIndexBuilt();
		return iterator(this, 0);
	}

	iterator end() {
		ensureIndexBuilt();
		return iterator(this, size());
	}
	// 删除从 pos 开始的 n 个字符
	void erase(size_t pos, size_t n = std::string::npos) {
		ensureIndexBuilt();
		if (pos >= utf8CharIndices.size())
			return;

		size_t byteStart = utf8CharIndices[pos];
		size_t byteEnd =
			(n == std::string::npos || pos + n >= utf8CharIndices.size()) ? str.length() : utf8CharIndices[pos + n];

		str.erase(byteStart, byteEnd - byteStart);
	}

	// 删除从 position 处的一个字符
	void erase(std::string::iterator position) {
		size_t pos = std::distance(str.begin(), position);
		erase(getCharPosition(pos), 1);
	}

	// 删除从 first 到 last 之间的字符
	void erase(std::string::iterator first, std::string::iterator last) {
		size_t startPos = std::distance(str.begin(), first);
		size_t endPos = std::distance(str.begin(), last);
		size_t startCharPos = getCharPosition(startPos);
		size_t endCharPos = getCharPosition(endPos);
		erase(startCharPos, endCharPos - startCharPos);
	}

	// 使用 Utf8String::iterator 的 erase 方法重载
	void erase(Utf8String::iterator position) {
		erase(position.getCharPosition(), 1);
	}

	void erase(Utf8String::iterator first, Utf8String::iterator last) {
		erase(first.getCharPosition(), last.getCharPosition() - first.getCharPosition());
	}

	// 添加字符串
	Utf8String &append(char ch) {
		str += ch;
		dirty = true;
		return *this;
	}

	// 添加字符串
	Utf8String &append(const std::string &s) {
		str.append(s);
		dirty = true;
		return *this;
	}

private:
	std::string str;							  // 原始字符串
	mutable std::vector<size_t> utf8CharIndices;  // 记录每个u8字符的起始位置
	mutable bool dirty;							  // 是否需要重新构建索引

private:
	/// 作用是根据 UTF-8 字符的位置（即字符索引）来找到相应的字节位置。这一点对于正确处理 UTF-8 编码的字符串非常重要，因为 UTF-8 编码的字符可能由不同数量的字节组成。这个函数允许您基于字符的索引而不是字节的索引来操作字符串。
	/// \param charPos 指定的 UTF-8 字符位置（字符索引）
	/// \param startPos 字节字符串中的起始搜索位置，默认值为 0。这允许函数从字符串的任意位置开始计数。
	/// \return 指定位置的字节位置（字节开始的索引）
	/// e.g. getBytePosition(0) == 0
	size_t getBytePosition(size_t charPos, size_t startPos = 0) const {
		size_t byteCount = startPos;
		size_t charCount = 0;
		while (byteCount < str.size()) {
			if (isUtf8StartByte(str[byteCount])) {
				if (charCount == charPos) {
					return byteCount;
				}
				++charCount;
			}
			++byteCount;
		}
		return str.size();	// 如果找不到，返回字符串的总长度
	}

	// 辅助函数，用于获取第 n 个 UTF-8 字符
	// warning: 可能不适用于所有 UTF-8 编码的复杂性(例如，它不处理 Unicode 的合成字符)
	// e.g. getUtf8Char(0) == "中"
	std::string getUtf8Char(size_t n) const {
		ensureIndexBuilt();
		size_t start = utf8CharIndices[n];
		size_t len = (n + 1 < utf8CharIndices.size()) ? utf8CharIndices[n + 1] - start : str.size() - start;
		return str.substr(start, len);
	}

	// 将string字节位置转换为utf8string字符位置(offset)
	// e.g. getCharPosition(0) == 0
	size_t getCharPosition(size_t bytePos) const {
		ensureIndexBuilt();
		auto it = std::lower_bound(utf8CharIndices.begin(), utf8CharIndices.end(), bytePos);
		if (it != utf8CharIndices.end()) {
			return std::distance(utf8CharIndices.begin(), it);
		}
		return utf8CharIndices.size();
	}

	void ensureIndexBuilt() const {
		if (dirty) {
			buildIndices();
			dirty = false;
		}
	}

	// 构建 UTF-8 字符的索引
	void buildIndices() const {
		utf8CharIndices.clear();
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

#endif	//TRANSLATEAPP_UTF8STRING_H

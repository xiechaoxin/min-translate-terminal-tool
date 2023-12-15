//
// Created by xcx on 2023/12/15.
//

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iterator>

class Utf8String {
public:
	Utf8String() = default;

	/// 为了使 Utf8String 类的 operator[] 具有 O(1) 的时间复杂度，我们可以在类初始化时预计算并存储每个 UTF-8 字符的起始位置。这可以通过使用一个 std::vector 来完成，该 vector 将保存字符串中每个 UTF-8 字符的起始索引。
	Utf8String(const std::string &s) : str(s) {
		for (size_t i = 0; i < str.size(); ++i) {
			if (isUtf8StartByte(str[i])) {
				utf8CharIndices.push_back(i);
				i += getUtf8CharLength(str[i]);
			}
		}
	}

	// warning: 可能不适用于所有 UTF-8 编码的复杂性(例如，它不处理 Unicode 的合成字符)
	std::string operator[](size_t n) const {
		if (n >= utf8CharIndices.size()) {
			throw std::out_of_range("Index out of range");
		}

		size_t start = utf8CharIndices[n];
		size_t len = (n + 1 < utf8CharIndices.size()) ? utf8CharIndices[n + 1] - start : str.size() - start;
		return str.substr(start, len);
	}

	size_t size() const {
		return utf8CharIndices.size();
	}

	// 类实现迭代器。这样的实现会让你能够遍历每个 UTF-8 编码的字符，就像遍历一个常规的字符数组一样。
		class iterator : public std::iterator<std::input_iterator_tag, std::string> {
		public:
			iterator(const Utf8String* u8str, size_t pos) : u8str(u8str), pos(pos) {}

			iterator& operator++() {
				++pos;
				return *this;
			}

			iterator operator++(int) {
				iterator tmp = *this;
				++(*this);
				return tmp;
			}

			bool operator==(const iterator& other) const {
				return pos == other.pos;
			}

			bool operator!=(const iterator& other) const {
				return pos != other.pos;
			}

			std::string operator*() const {
				return (*u8str)[pos];
			}

		private:
			const Utf8String* u8str;
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
		if ((c & 0x80) == 0) return 1;
		if ((c & 0xE0) == 0xC0) return 2;
		if ((c & 0xF0) == 0xE0) return 3;
		if ((c & 0xF8) == 0xF0) return 4;
		return 1; // Should not reach here if valid UTF-8
	}
};

int main() {
	std::string str = "中aHello, 世界bcd文";  // UTF-8 字符串
	Utf8String utf8str(str);
	std::cout << utf8str.size() << std::endl;
	for (const auto &item : utf8str) {
		std::cout << item << std::endl;
	}
	return 0;
}

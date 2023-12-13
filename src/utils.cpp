#include "utils.h"
#include <ctype.h>

bool is_ascii(const std::string &word) {
	for (const auto &item : word) {
		if (!isascii(item))
			return false;
	}
	return true;
}

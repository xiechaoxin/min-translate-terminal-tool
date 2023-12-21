#include <iostream>
#include <vector>
#include <string>
#include <regex>

std::vector<std::string> splitWord(const std::string& word) {
    std::vector<std::string> result;
    std::regex re("([A-Z]?[a-z]+)|([A-Z][A-Z]+(?=[A-Z][a-z]|$))|([0-9]+)");
    std::smatch match;

    std::string::const_iterator searchStart(word.cbegin());
    while (std::regex_search(searchStart, word.cend(), match, re)) {
        result.push_back(match.str(0)); // 只添加匹配的整个字符串
        searchStart = match.suffix().first;
    }

    return result;
}

int main() {
    std::string word = "Splitted..__..words";
    std::vector<std::string> splittedWords = splitWord(word);

    for (const std::string& w : splittedWords) {
        std::cout << w << std::endl;
    }

    return 0;
}

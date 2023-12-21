#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {
    std::string line;
    std::ifstream file("words.txt");
    std::vector<std::string> words;

    if (file.is_open()) {
        while (getline(file, line)) {
            words.push_back(line);
        }
        file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    // 处理或输出读取到的词
    for (const auto& word : words) {
        std::cout << word << std::endl;
    }

    return 0;
}

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <mutex>
#include <sstream>
#include "utils.h"
#include "dictionary.h"
#include "search.h"


int main() {
	std::string input;
	std::thread processingThread;  // 使用单个线程而非线程数组

	init_db();

	while (true) {
		std::cout << "请输入文本（回车结束输入, 如果输入为空, 程序结束, "
					 "您也可以手动结束<Ctrl-c>）：\n";
		std::getline(std::cin, input);
		if (input.empty()) {
			break;
		}

		// 等待之前的线程完成
		if (processingThread.joinable()) {
			processingThread.join();
		}

		// 启动一个新线程来处理当前的输入
		processingThread = std::thread(processInput, input);
	}

	// 确保最后一个线程也完成了
	if (processingThread.joinable()) {
		processingThread.join();
	}

	std::cout << "输入结束。" << std::endl;

	return 0;
}

#include <Util.h>

#include <regex>
#include <iostream>
#include <fstream>

namespace util {
	std::string readFile(const std::string &path) {
		std::ifstream stream(path);

		std::string fileData = "";

		if (stream.is_open()) {
			while (stream.good() && !stream.eof()) {
				std::string line = "";
				std::getline(stream, line);
				fileData += line + "\n";
			}
		}
		else
			std::cerr << "[Error] Cannot open path: " << path << std::endl;

		return fileData;
	}

	std::string replaceAll(const std::string &str, const std::string &regex, const std::string &insert) {
		std::string newStr = "";

		std::regex reg = std::regex(regex);
		std::smatch m;
		std::string sub = "";
		for (int i = 0; i < str.length(); i++) {
			sub += str[i];
			if (std::regex_search(sub, m, reg)) {
				newStr += sub.substr(0, m.position()) + insert;
				sub = "";
			}
		}
		if (!sub.empty())
			newStr += sub;

		return newStr;
	}
}
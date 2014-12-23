#include <string>
#include <iostream>

namespace util {
	std::string readFile(const std::string &path);

	std::string replaceAll(const std::string &str, const std::string &regex, const std::string &insert);
}
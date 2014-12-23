#include <JSExtensions.h>

#include <iostream>
#include <string>

namespace js {
	namespace funcs {
		void print(const CFunctionsScopePtr &v, void* extraData) {
			std::cout << "[JS] " << v->getArgument("str")->getString() << std::endl;
		}
	}
}
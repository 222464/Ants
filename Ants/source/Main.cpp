#include <js/Javascript.h>

#include <memory>
#include <iostream>
#include <string>

void js_print(const CFunctionsScopePtr &v, void* extraData) {
	std::cout << v->getArgument("text")->getString().c_str() << std::endl;
}

int main() {
	std::shared_ptr<CTinyJS> js = std::make_shared<CTinyJS>();

	/* add the functions from TinyJS_Functions.cpp */
	registerFunctions(js.get());
	registerStringFunctions(js.get());
	registerMathFunctions(js.get());

	/* Add a native function */
	js->addNative("function print(text)", js_print, 0);

	js->executeSafe("var lets_quit = 0; function quit() { lets_quit = 1; }");
	js->executeSafe("print(\"Interactive mode... Type quit(); to exit, or print(...); to print something.\");");
	
	int lineNumber = 0;
	while (js->evaluate("lets_quit") == "0") {
		std::string buffer;
		std::cout << ">";
		std::getline(std::cin, buffer);
		js->executeSafe(buffer, "stdin", lineNumber++);
	}

	system("pause");
	return 0;
}

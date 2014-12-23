#include <ants/objects/JSAnt.h>

#include <Util.h>
#include <regex>

using namespace ants;

JSAnt::JSAnt(Ptr<CTinyJS> js, const std::string &jsFilePath) {
	_js = js;
	_jsFilePath = jsFilePath;
	_objName = "";
}

JSAnt::~JSAnt() {
}

void JSAnt::onAdd() {
	World* pWorld = getWorld();

	if (!pWorld->_resources.exists("jsant")) {
		_jsAntTexture = make<sf::Texture>();

		_jsAntTexture->loadFromFile("resources/testAnt.png");

		pWorld->_resources.insertPtr("jsant", _jsAntTexture);
	}
	else
		_jsAntTexture = pWorld->_resources.getPtr<sf::Texture>("testant");

	//Load js code.
	if (!pWorld->_resources.exists(_jsFilePath)) {
		_jsCode = make<std::string>(util::readFile(_jsFilePath));

		pWorld->_resources.insertPtr(_jsFilePath, _jsCode);
	}
	else
		_jsCode = pWorld->_resources.getPtr<std::string>(_jsFilePath);

	//Do not allow returns in global scope.
	std::string code = (*_jsCode);
	int scopeC = 0;
	std::string sub = "";
	std::regex reg = std::regex("return.*;");
	for (int i = 0; i < code.length(); i++) {
		if (code[i] == '{')
			scopeC++;
		else if (code[i] == '}') {
			scopeC--;
			if (scopeC == 0)
				sub = "";
		}
		if (scopeC == 0) {
			sub += code[i];
			if (std::regex_search(sub, reg)) {
				(*_jsCode) = "function onAdd(){} function update(){}";
				break;
			}
		}
	}

	_objName = util::replaceAll(_jsFilePath, "/", "_");
	_objName = util::replaceAll(_objName, "\.js", "");

	std::string exeStr = "var " +
		_objName +
		"=(function(){" +
		*_jsCode +
		"return {onAdd: onAdd, update: update};" + 
		"})();";
	_js->executeSafe(exeStr);

	_js->executeSafe(_objName + ".onAdd();");
}

void JSAnt::update() {
	std::uniform_int_distribution<int> dirDist(-1, 1);

	int dx = dirDist(getWorld()->_generator);
	int dy = dirDist(getWorld()->_generator);

	setPosition(sf::Vector2i(getPosition().x + dx, getPosition().y + dy));

	placeMarker(0, sf::Vector2f(dx, dy));

	_js->executeSafe(_objName + ".update();");
}

void JSAnt::render(sf::RenderTarget &target) {
	sf::Sprite sprite;
	sprite.setTexture(*_jsAntTexture);

	sprite.setPosition(sf::Vector2f(getPosition().x * getWorld()->getCellWidth(), getPosition().y * getWorld()->getCellHeight()));

	target.draw(sprite);
}
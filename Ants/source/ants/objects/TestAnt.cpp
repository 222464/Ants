#include <ants/objects/TestAnt.h>

using namespace ants;

void TestAnt::onAdd() {
	World* pWorld = getWorld();

	if (!pWorld->_resources.exists("testant")) {
		_testAntTexture = std::make_shared<sf::Texture>();

		_testAntTexture->loadFromFile("resources/testAnt.png");

		pWorld->_resources.insertPtr("testant", _testAntTexture);
	}
	else
		_testAntTexture = pWorld->_resources.getPtr<sf::Texture>("testant");
}

void TestAnt::update() {
	std::uniform_int_distribution<int> dirDist(-1, 1);

	int dx = dirDist(getWorld()->_generator);
	int dy = dirDist(getWorld()->_generator);

	setPosition(sf::Vector2i(getPosition().x + dx, getPosition().y + dy));

	placeMarker(0, sf::Vector2f(dx, dy));
}

void TestAnt::render(sf::RenderTarget &target) {
	sf::Sprite sprite;
	sprite.setTexture(*_testAntTexture);

	sprite.setPosition(sf::Vector2f(getPosition().x * getWorld()->getCellWidth(), getPosition().y * getWorld()->getCellHeight()));

	target.draw(sprite);
}
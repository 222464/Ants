#include <ants/objects/Food.h>

using namespace ants;

void Food::onAdd() {
	World* pWorld = getWorld();

	if (!pWorld->_resources.exists("food")) {
		Ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

		texture->loadFromFile("resources/food.png");

		_foodTexture = texture.get();

		pWorld->_resources.insertPtr("food", texture);
	}
	else
		_foodTexture = pWorld->_resources.getPtr<sf::Texture>("food");
}

void Food::update() {
	if (_amount <= 0)
		getWorld()->remove(getObjectId());
}

void Food::render(sf::RenderTarget &target) {
	sf::Sprite sprite;
	sprite.setTexture(*_foodTexture);

	sprite.setPosition(sf::Vector2f(getPosition().x * getWorld()->getCellWidth(), getPosition().y * getWorld()->getCellHeight()));

	target.draw(sprite);
}
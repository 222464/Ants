#include <ants/objects/Nest.h>

using namespace ants;

void Nest::onAdd() {
	World* pWorld = getWorld();

	if (!pWorld->_resources.exists("nest")) {
		Ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

		texture->loadFromFile("resources/nest.png");

		_nestTexture = texture.get();

		pWorld->_resources.insertPtr("nest", texture);
	}
	else
		_nestTexture = pWorld->_resources.getPtr<sf::Texture>("nest");
}

void Nest::update() {
}

void Nest::render(sf::RenderTarget &target) {
	sf::Sprite sprite;
	sprite.setTexture(*_nestTexture);

	sprite.setPosition(sf::Vector2f(getPosition().x * getWorld()->getCellWidth(), getPosition().y * getWorld()->getCellHeight()));

	target.draw(sprite);
}
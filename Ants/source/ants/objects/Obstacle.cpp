#include <ants/objects/Obstacle.h>

using namespace ants;

Obstacle::~Obstacle() {
	if (getWorld() != nullptr) {
		for (std::set<int>::const_iterator cit = getWorld()->getCell(getPosition())._occupantIds.begin(); cit != getWorld()->getCell(getPosition())._occupantIds.end(); cit++) {
			if (getWorld()->getGridObject(*cit)->getName() == "obstacle")
				return;
		}

		getWorld()->getCell(getPosition())._passable = true;
	}
}

void Obstacle::onAdd() {
	World* pWorld = getWorld();

	if (!pWorld->_resources.exists("obstacle")) {
		Ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

		texture->loadFromFile("resources/obstacle.png");

		_obstacleTexture = texture.get();

		pWorld->_resources.insertPtr("obstacle", texture);
	}
	else
		_obstacleTexture = pWorld->_resources.getPtr<sf::Texture>("obstacle");

	pWorld->getCell(getPosition())._passable = false;
}

void Obstacle::update() {
}

void Obstacle::render(sf::RenderTarget &target) {
	sf::Sprite sprite;
	sprite.setTexture(*_obstacleTexture);

	sprite.setPosition(sf::Vector2f(getPosition().x * getWorld()->getCellWidth(), getPosition().y * getWorld()->getCellHeight()));

	target.draw(sprite);
}
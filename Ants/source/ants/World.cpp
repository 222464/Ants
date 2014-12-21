#include <ants/World.h>

#include <assert.h>

using namespace ants;

World::~World() {
	for (std::unordered_map<int, std::shared_ptr<GridObject>>::iterator it = _gridObjects.begin(); it != _gridObjects.end(); it++)
		it->second->_pWorld = nullptr;
}

void World::create(int width, int height, const std::vector<sf::Color> &markerGroupColors, const std::shared_ptr<sf::Texture> &gridTexture) {
	_width = width;
	_height = height;

	_grid.resize(width * height);

	_gridTexture = gridTexture;

	_markerGroupColors = markerGroupColors;
}

void World::add(const std::shared_ptr<GridObject> &gridObject, sf::Vector2i &position) {
	gridObject->_objectId = ++_lastObjectId;

	_gridObjects[gridObject->_objectId] = gridObject;

	gridObject->_position = position;
	gridObject->_pWorld = this;

	getCell(position)._occupantIds.insert(gridObject->_objectId);

	gridObject->onAdd();
}

void World::remove(int objectId) {
	std::unordered_map<int, std::shared_ptr<GridObject>>::iterator it = _gridObjects.find(objectId);

	assert(it != _gridObjects.end());

	_gridObjects.erase(it);

	getCell(it->second->getPosition())._occupantIds.erase(objectId);

	it->second->_pWorld = nullptr;
}

void World::setPosition(int objectId, const sf::Vector2i &position) {
	if (position.x < 0 || position.y < 0 || position.x >= getWidth() || position.y >= getHeight())
		return;

	std::shared_ptr<GridObject> obj = getGridObject(objectId);

	getCell(obj->getPosition())._occupantIds.erase(objectId);

	obj->_position = position;
	
	getCell(obj->getPosition())._occupantIds.insert(objectId);
}

void World::placeMarker(const Marker &mark, sf::Vector2i &position) {
	getCell(position)._markers.push_back(mark);
}

void World::update() {
	float markerMult = 1.0f - _markDecay;

	// Remove too weak markers
	for (int x = 0; x < _width; x++)
	for (int y = 0; y < _height; y++) {
		sf::Vector2i pos(x, y);

		std::vector<Marker> leftOvers;

		Cell &cell = getCell(pos);

		for (int m = 0; m < cell._markers.size(); m++) {
			cell._markers[m]._vector *= markerMult;

			if (vectorMagnitude(cell._markers[m]._vector) > _markTolerance)
				leftOvers.push_back(cell._markers[m]);
		}

		cell._markers = leftOvers;
	}

	for (std::unordered_map<int, std::shared_ptr<GridObject>>::iterator it = _gridObjects.begin(); it != _gridObjects.end(); it++)
		it->second->update();
}

void World::render(sf::RenderTarget &target) {
	const float colorInv = 1.0f / 255.0f;

	sf::Sprite gridSprite;

	gridSprite.setTexture(*_gridTexture);

	sf::View view = target.getView();

	// Get view rect
	sf::FloatRect viewRect = rectFromBounds(view.getCenter(), view.getCenter());

	viewRect = rectExpand(viewRect, target.mapPixelToCoords(sf::Vector2i(0, 0)));
	viewRect = rectExpand(viewRect, target.mapPixelToCoords(sf::Vector2i(target.getSize().x, 0)));
	viewRect = rectExpand(viewRect, target.mapPixelToCoords(sf::Vector2i(target.getSize().x, target.getSize().y)));
	viewRect = rectExpand(viewRect, target.mapPixelToCoords(sf::Vector2i(0, target.getSize().y)));

	sf::Vector2f viewLower = rectLowerBound(viewRect);
	sf::Vector2f viewUpper = rectUpperBound(viewRect);

	int minSx = std::max(0, static_cast<int>(viewLower.x / 512.0f));
	int minSy = std::max(0, static_cast<int>(viewLower.y / 512.0f));
	int maxSx = std::min(_width / 64, 1 + static_cast<int>(std::ceil(1 + viewUpper.x / 512.0f)));
	int maxSy = std::min(_height / 64, 1 + static_cast<int>(std::ceil(1 + viewUpper.y / 512.0f)));

	for (int sx = minSx; sx < maxSx; sx++)
	for (int sy = minSy; sy < maxSy; sy++) {
		gridSprite.setPosition(sf::Vector2f(sx * 512.0f, sy * 512.0f));

		target.draw(gridSprite);
	}

	sf::RenderStates markerRenderStates;
	markerRenderStates.blendMode = sf::BlendMultiply;

	for (int x = 0; x < _width; x++)
	for (int y = 0; y < _height; y++) {
		sf::Vector2i pos(x, y);

		Cell &cell = getCell(pos);

		if (!cell._markers.empty()) {
			float r = 1.0f;
			float g = 1.0f;
			float b = 1.0f;
			float totalMag = 1.0f;

			for (int m = 0; m < cell._markers.size(); m++) {
				float mag = vectorMagnitude(cell._markers[m]._vector);

				r += mag * _markerGroupColors[cell._markers[m]._groupId].r * colorInv;
				g += mag * _markerGroupColors[cell._markers[m]._groupId].g * colorInv;
				b += mag * _markerGroupColors[cell._markers[m]._groupId].b * colorInv;

				totalMag += mag;
			}

			float totalMagInv = 1.0f / totalMag;

			sf::RectangleShape rectShape;

			rectShape.setPosition(sf::Vector2f(pos.x * getCellWidth() + 1, pos.y * getCellHeight() + 1));
			rectShape.setSize(sf::Vector2f(getCellWidth() - 1.0f, getCellHeight() - 1.0f));
			rectShape.setFillColor(sf::Color(r * totalMagInv * 255.0f, g * totalMagInv * 255.0f, b * totalMagInv * 255.0f));

			target.draw(rectShape);
		}
	}

	for (std::unordered_map<int, std::shared_ptr<GridObject>>::iterator it = _gridObjects.begin(); it != _gridObjects.end(); it++) {
		sf::Vector2f lowerBound = sf::Vector2f(it->second->getPosition().x * getCellWidth(), it->second->getPosition().y * getCellHeight());
		sf::Vector2f upperBound = lowerBound + getCellSize();

		sf::FloatRect aabb = rectFromBounds(lowerBound, upperBound);

		if (rectIntersects(viewRect, aabb))
			it->second->render(target);
	}
}
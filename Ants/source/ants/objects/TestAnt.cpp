#include <ants/objects/TestAnt.h>

#include <ants/objects/Food.h>
#include <ants/objects/Nest.h>

#include <array>

#include <assert.h>

using namespace ants;

void TestAnt::onAdd() {
	World* pWorld = getWorld();

	if (!pWorld->_resources.exists("testant")) {
		Ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

		texture->loadFromFile("resources/testAnt.png");

		_testAntTexture = texture.get();

		pWorld->_resources.insertPtr("testant", texture);
	}
	else
		_testAntTexture = pWorld->_resources.getPtr<sf::Texture>("testant");

	if (!pWorld->_resources.exists("testantwf")) {
		Ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

		texture->loadFromFile("resources/testAntWithFood.png");

		_testAntWithFoodTexture = texture.get();

		pWorld->_resources.insertPtr("testantwf", texture);
	}
	else
		_testAntWithFoodTexture = pWorld->_resources.getPtr<sf::Texture>("testantwf");

	std::uniform_int_distribution<int> dirDist(0, 7);

	_dir = dirDist(pWorld->_generator);

	_renderPosition = sf::Vector2f(getPosition().x, getPosition().y);
	_renderRotation = _dir * 45.0f;
}

void TestAnt::update() {
	std::uniform_real_distribution<float> dist01(0.0f, 1.0f);
	std::uniform_int_distribution<int> pertDist(0, _maxDirPert);

	World::Cell &centerCell = getWorld()->getCell(getPosition());

	for (std::set<int>::const_iterator cit = centerCell._occupantIds.begin(); cit != centerCell._occupantIds.end(); cit++) {
		Ptr<GridObject> go = getWorld()->getGridObject(*cit);

		if (go->getName() == "nest") {
			if (_mode == _retrieve) {
				Ptr<Nest> nest = std::static_pointer_cast<Nest>(go);

				nest->_amount++;

				_dir = (_dir + 4) % 8;
			}

			_mode = _search;
		}
		else if (go->getName() == "food") {
			if (_mode == _search) {
				Ptr<Food> food = std::static_pointer_cast<Food>(go);

				food->_amount--;

				_dir = (_dir + 4) % 8;
			}

			_mode = _retrieve;
		}
	}

	float trackingMag = 0.0f;

	if (_mode == _search) {
		const int markerGroupId = 1;

		sf::Vector2f vector(0.0f, 0.0f);

		for (int i = 0; i < centerCell._markers.size(); i++) {
			if (centerCell._markers[i]._groupId == markerGroupId)
				vector += centerCell._markers[i]._vector;
		}

		float mag = vectorMagnitude(vector);

		if (mag > getWorld()->_markTolerance) {
			int newDir = std::round(std::atan2(vector.y, vector.x) / _pi * 4);

			newDir = newDir % 8;

			if (newDir < 0)
				newDir += 8;

			int leftAngle = newDir - _dir;
			int rightAngle = _dir + (8 - newDir);

			if (leftAngle < rightAngle) {
				_dir += std::min(_maxMarkerTurn, leftAngle);
			}
			else {
				_dir -= std::min(_maxMarkerTurn, rightAngle);
			}

			_dir = _dir % 8;

			if (_dir < 0)
				_dir += 8;

			trackingMag = mag;
		}
	}
	else {
		const int markerGroupId = 0;

		sf::Vector2f vector(0.0f, 0.0f);

		for (int i = 0; i < centerCell._markers.size(); i++) {
			if (centerCell._markers[i]._groupId == markerGroupId)
				vector += centerCell._markers[i]._vector;
		}

		float mag = vectorMagnitude(vector);

		if (mag > getWorld()->_markTolerance) {
			int newDir = std::round(std::atan2(vector.y, vector.x) / _pi * 4);

			newDir = newDir % 8;

			if (newDir < 0)
				newDir += 8;

			int leftAngle = newDir - _dir;
			int rightAngle = _dir + (8 - newDir);

			if (leftAngle < rightAngle) {
				_dir += std::min(_maxMarkerTurn, leftAngle);
			}
			else {
				_dir -= std::min(_maxMarkerTurn, rightAngle);
			}

			_dir = _dir % 8;

			if (_dir < 0)
				_dir += 8;

			trackingMag = mag;
		}
	}

	if (trackingMag <= getWorld()->_markTolerance) {
		if (dist01(getWorld()->_generator) < _pertDirChance) {
			_dir += pertDist(getWorld()->_generator) * 2 - _maxDirPert;

			_dir = _dir % 8;

			if (_dir < 0)
				_dir += 8;
		}
	}
	else {
		if (dist01(getWorld()->_generator) < std::max(_minPertDirChance, _pertDirWhenOnTrackChance / (1.0f + _pertMarkerMagnitudeScalar * trackingMag))) {
			_dir += pertDist(getWorld()->_generator) * 2 - _maxDirPert;

			_dir = _dir % 8;

			if (_dir < 0)
				_dir += 8;
		}
	}

	float dxf = std::cos(_dir * _pi * 0.25f);
	float dyf = std::sin(_dir * _pi * 0.25f);

	int dx = std::abs(dxf) < 0.01f ? 0 : (dxf > 0.0f ? 1 : -1);
	int dy = std::abs(dyf) < 0.01f ? 0 : (dyf > 0.0f ? 1 : -1);

	//getWorld()->getCell(getPosition())._passable = true;

	sf::Vector2i oldPosition = getPosition();

	setPosition(sf::Vector2i(getPosition().x + dx, getPosition().y + dy));

	//getWorld()->getCell(getPosition())._passable = false;

	if (getPosition() != oldPosition) {
		if (_mode == _search)
			placeMarker(0, sf::Vector2f(-dx, -dy));
		else
			placeMarker(1, sf::Vector2f(-dx, -dy));
	}
}

void TestAnt::renderUpdate() {
	sf::Vector2f newPositionf = sf::Vector2f(getPosition().x, getPosition().y);
	float newRotationf = _dir * 45.0f;

	const float interpolateMult = 0.2f;

	_renderPosition += (newPositionf - _renderPosition) * interpolateMult;

	float leftAngle = newRotationf - _renderRotation;
	float rightAngle = _renderRotation + (360.0f - newRotationf);

	if (leftAngle < rightAngle)
		_renderRotation += leftAngle * interpolateMult;
	else
		_renderRotation -= rightAngle * interpolateMult;
}

void TestAnt::render(sf::RenderTarget &target) {
	if (_mode == _retrieve) {
		sf::Sprite sprite;

		sprite.setOrigin(sf::Vector2f(getWorld()->getCellWidth() * 0.5f, getWorld()->getCellHeight() * 0.5f));

		sprite.setTexture(*_testAntWithFoodTexture);

		sprite.setRotation(_renderRotation + 90.0f);

		sprite.setPosition(sf::Vector2f((_renderPosition.x + 0.5f) * getWorld()->getCellWidth(), (_renderPosition.y + 0.5f) * getWorld()->getCellHeight()));

		target.draw(sprite);
	}
	else {
		sf::Sprite sprite;

		sprite.setOrigin(sf::Vector2f(getWorld()->getCellWidth() * 0.5f, getWorld()->getCellHeight() * 0.5f));

		sprite.setTexture(*_testAntTexture);

		sprite.setRotation(_renderRotation + 90.0f);

		sprite.setPosition(sf::Vector2f((_renderPosition.x + 0.5f) * getWorld()->getCellWidth(), (_renderPosition.y + 0.5f) * getWorld()->getCellHeight()));

		target.draw(sprite);
	}
}
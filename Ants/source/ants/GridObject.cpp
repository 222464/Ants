#include <ants/GridObject.h>

#include <ants/World.h>

using namespace ants;

GridObject::~GridObject() {
	if (_pWorld != nullptr)
		_pWorld->remove(_objectId);
}

void GridObject::setPosition(const sf::Vector2i &position) {
	if (position.x < 0 || position.y < 0 || position.x >= getWorld()->getWidth() || position.y >= getWorld()->getHeight())
		return;

	if (!_pWorld->getCell(position)._passable)
		return;

	_position = position;

	_pWorld->setPosition(_objectId, position);
}

void GridObject::placeMarker(int groupId, const sf::Vector2f &vector) {
	World::Marker marker;

	marker._groupId = groupId;
	marker._individualId = _objectId;
	marker._vector = vector;

	getWorld()->placeMarker(marker, _position);
}
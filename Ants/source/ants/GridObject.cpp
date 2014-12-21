#include <ants/GridObject.h>

#include <ants/World.h>

using namespace ants;

GridObject::~GridObject() {
	if (_pWorld != nullptr)
		_pWorld->remove(_objectId);
}

void GridObject::setPosition(const sf::Vector2i &position) {
	_position = position;

	if (_pWorld->getCell(position)._passable)
		_pWorld->setPosition(_objectId, position);
}

void GridObject::placeMarker(int groupId, const sf::Vector2f &mark) {
	World::Marker marker;

	marker._groupId = groupId;
	marker._individualId = _objectId;
	marker._vector = mark;

	getWorld()->placeMarker(marker, _position);
}
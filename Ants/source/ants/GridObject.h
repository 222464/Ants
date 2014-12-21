#pragma once

#include <ants/AntMath.h>
#include <SFML/Graphics.hpp>

namespace ants {
	class GridObject {
	private:
		class World* _pWorld;
		sf::Vector2i _position;
		int _objectId;

	public:
		~GridObject();

		const sf::Vector2i &getPosition() {
			return _position;
		}

		void setPosition(const sf::Vector2i &position);

		virtual void onAdd() {}
		virtual void update() {}
		virtual void render(sf::RenderTarget &target) {}

		void placeMarker(int groupId, const sf::Vector2f &mark);

		class World* getWorld() const {
			return _pWorld;
		}

		friend class World;
	};
}
#pragma once

#include <ants/AntMath.h>
#include <SFML/Graphics.hpp>

namespace ants {
	class GridObject {
	private:
		class World* _pWorld;
		sf::Vector2i _position;
		int _objectId;

	protected:
		std::string _name;

		bool _removeMe;

	public:
		GridObject()
			: _removeMe(false)
		{}

		~GridObject();

		const sf::Vector2i &getPosition() {
			return _position;
		}

		void setPosition(const sf::Vector2i &position);

		virtual void onAdd() {}
		virtual void update() {}
		virtual void renderUpdate() {}
		virtual void render(sf::RenderTarget &target) {}

		void placeMarker(int groupId, const sf::Vector2f &vector);

		class World* getWorld() const {
			return _pWorld;
		}

		const std::string &getName() const {
			return _name;
		}

		int getObjectId() const {
			return _objectId;
		}

		void remove() {
			_removeMe = true;
		}

		friend class World;
	};
}
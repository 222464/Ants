#pragma once

#include <SFML/Graphics.hpp>

namespace ants {
	class World {
	private:
		struct Cell {
			float _pheromoneA;
			float _pheromoneB;

			bool _passable;
		};

		sf::Image _gridImage;

	public:
	};
}
#pragma once

#include <ants/GridObject.h>

#include <ants/World.h>

namespace ants {
	class Obstacle : public GridObject {
	private:
		sf::Texture* _obstacleTexture;

	public:
		Obstacle() {
			_name = "obstacle";
		}

		~Obstacle();

		// Inherited from GridObject
		void onAdd();
		void update();
		void render(sf::RenderTarget &target);
	};

}
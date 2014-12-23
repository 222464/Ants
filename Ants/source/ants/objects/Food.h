#pragma once

#include <ants/GridObject.h>

#include <ants/World.h>

namespace ants {
	class Food : public GridObject {
	private:
		sf::Texture* _foodTexture;

	public:
		int _amount;

		Food()
			: _amount(80)
		{
			_name = "food";
		}

		// Inherited from GridObject
		void onAdd();
		void update();
		void render(sf::RenderTarget &target);
	};

}
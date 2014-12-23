#pragma once

#include <ants/GridObject.h>

#include <ants/World.h>

namespace ants {
	class Nest : public GridObject {
	private:
		sf::Texture* _nestTexture;

	public:
		int _amount;

		Nest()
			: _amount(0)
		{
			_name = "nest";
		}

		// Inherited from GridObject
		void onAdd();
		void update();
		void render(sf::RenderTarget &target);
	};

}
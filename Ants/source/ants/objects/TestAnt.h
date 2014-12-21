#pragma once

#include <ants/GridObject.h>

#include <ants/World.h>

namespace ants {
	class TestAnt : public GridObject {
	private:
		Ptr<sf::Texture> _testAntTexture;

	public:
		// Inherited from GridObject
		void onAdd();
		void update();
		void render(sf::RenderTarget &target);
	};

}
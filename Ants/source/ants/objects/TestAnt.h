#pragma once

#include <ants/GridObject.h>

#include <ants/World.h>

namespace ants {
	class TestAnt : public GridObject {
	public:
		enum Mode {
			_search, _retrieve
		};

	private:
		sf::Texture* _testAntTexture;
		sf::Texture* _testAntWithFoodTexture;

		Mode _mode;

		int _dir;

		sf::Vector2f _renderPosition;
		float _renderRotation;

	public:
		float _pertDirChance;
		float _pertDirWhenOnTrackChance;
		float _pertMarkerMagnitudeScalar;
		float _minPertDirChance;
		int _maxDirPert;
		int _maxMarkerTurn;

		TestAnt()
			: _mode(_search),
			_pertDirChance(0.5f), _pertDirWhenOnTrackChance(0.3f), _maxDirPert(3), _pertMarkerMagnitudeScalar(4.0f), _minPertDirChance(0.04f), _maxMarkerTurn(2)
		{
			_name = "testant";
		}

		// Inherited from GridObject
		void onAdd();
		void update();
		void renderUpdate();
		void render(sf::RenderTarget &target);

		Mode getMode() const {
			return _mode;
		}
	};

}
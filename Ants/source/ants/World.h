#pragma once

#include <ants/GridObject.h>

#include <Table.h>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <random>

namespace ants {
	class World {
	public:
		struct Marker {
			sf::Vector2f _vector;
			int _groupId;
			int _individualId;

			Marker()
				: _groupId(-1), _individualId(-1), _vector(0.0f, 0.0f)
			{}
		};

		struct Cell {
			std::vector<Marker> _markers;

			std::set<int> _occupantIds;

			int _typeId;

			bool _passable;

			Cell()
				: _typeId(0), _passable(true)
			{}
		};

	private:
		std::vector<Cell> _grid;

		int _width, _height;

		std::unordered_map<int, std::shared_ptr<GridObject>> _gridObjects;

		int _lastObjectId;

		std::shared_ptr<sf::Texture> _gridTexture;

		std::vector<sf::Color> _markerGroupColors;

	public:
		Table _resources;

		std::mt19937 _generator;

		float _markTolerance;
		float _markDecay;

		float _markerRenderScalar;

		World()
			: _lastObjectId(0), _markTolerance(0.06f), _markDecay(0.008f), _markerRenderScalar(0.33f)
		{}

		~World();

		void create(int width, int height, const std::vector<sf::Color> &markerGroupColors, const std::shared_ptr<sf::Texture> &gridTexture);

		void add(const std::shared_ptr<GridObject> &gridObject, sf::Vector2i &position);
		void remove(int objectId);

		Cell &getCell(const sf::Vector2i &position) {
			return _grid[position.x + position.y * _width];
		}

		bool exists(int objectId) const {
			return _gridObjects.find(objectId) != _gridObjects.end();
		}

		const std::shared_ptr<GridObject> &getGridObject(int objectId) {
			return _gridObjects[objectId];
		}

		void setPosition(int objectId, const sf::Vector2i &position);

		void placeMarker(const Marker &mark, sf::Vector2i &position);

		void update();
		void render(sf::RenderTarget &target);

		int getWidth() const {
			return _width;
		}

		int getHeight() const {
			return _height;
		}

		float getCellWidth() const {
			return 8.0f;
		}

		float getCellHeight() const {
			return 8.0f;
		}

		sf::Vector2f getCellSize() const {
			return sf::Vector2f(8.0f, 8.0f);
		}
	};
}
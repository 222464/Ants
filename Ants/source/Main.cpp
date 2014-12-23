#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <ants/World.h>
#include <ants/objects/JSAnt.h>

#include <JSExtensions.h>
#include <Util.h>

#include <string>

#include <ants/objects/Food.h>
#include <ants/objects/Nest.h>
#include <ants/objects/Obstacle.h>

int main() {
	Ptr<CTinyJS> js = make<CTinyJS>();

	registerFunctions(js.get());
	registerMathFunctions(js.get());
	registerStringFunctions(js.get());

	//Add print function.
	js->addNative("function print(str)", js::funcs::print, 0);

	//Create console object.
	js->executeSafe("var console = {log: print};console.log('Console created.');");

	sf::RenderWindow window;
	window.create(sf::VideoMode(1280, 800), "ALife");

	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	sf::Clock clock;

	float dt = 0.017f;

	ants::World world;

	std::shared_ptr<sf::Texture> gridTexture = std::make_shared<sf::Texture>();

	gridTexture->loadFromFile("resources/grid.png");

	std::vector<sf::Color> colors(4);
	colors[0] = sf::Color::Cyan;
	colors[1] = sf::Color::Red;
	colors[2] = sf::Color::Magenta;
	colors[3] = sf::Color::Green;

	world.create(128, 128, colors, gridTexture);

	for (int i = 0; i < 5; i++) {
		std::uniform_int_distribution<int> nestDistX(1, world.getWidth() - 2);
		std::uniform_int_distribution<int> nestDistY(1, world.getHeight() - 2);

		sf::Vector2i pos = sf::Vector2i(nestDistX(world._generator), nestDistY(world._generator));

		for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			Ptr<ants::Nest> nest = make<ants::Nest>();

			world.add(nest, sf::Vector2i(pos.x + dx, pos.y + dy));
		}

		for (int i = 0; i < 50; i++) {
			Ptr<ants::TestAnt> testAnt = make<ants::TestAnt>();

			world.add(testAnt, pos);
		}
	}

	for (int i = 0; i < 8; i++) {
		std::uniform_int_distribution<int> foodDistX(1, world.getWidth() - 2);
		std::uniform_int_distribution<int> foodDistY(1, world.getHeight() - 2);

		sf::Vector2i pos = sf::Vector2i(foodDistX(world._generator), foodDistY(world._generator));

		for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			Ptr<ants::Food> food = make<ants::Food>();

			world.add(food, sf::Vector2i(pos.x + dx, pos.y + dy));
		}
	}

	Ptr<ants::JSAnt> jsAnt = std::make_shared<ants::JSAnt>(js, "js/ant.js");

	world.add(jsAnt, sf::Vector2i(32, 32));

	sf::View view;
	sf::View newView;

	view.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	view.setCenter(sf::Vector2f(world.getWidth() * 0.5f * world.getCellWidth(), world.getHeight() * 0.5f * world.getCellHeight()));

	newView = view;

	float zoomRate = 0.2f;

	float viewInterpolateRate = 20.0f;

	sf::Vector2i prevMousePos = sf::Mouse::getPosition();

	float updateTimer = 0.0f;
	const float updateTime = 0.1f;

	bool quit = false;
	
	do {
		sf::Event windowEvent;

		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				quit = true;

				break;

			case sf::Event::MouseWheelMoved:
				int dWheel = windowEvent.mouseWheel.delta;

				newView = view;

				window.setView(newView);

				sf::Vector2f mouseZoomDelta0 = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				newView.setSize(view.getSize() + newView.getSize() * (-zoomRate * dWheel));

				window.setView(newView);

				sf::Vector2f mouseZoomDelta1 = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				window.setView(view);

				newView.setCenter(view.getCenter() + mouseZoomDelta0 - mouseZoomDelta1);

				break;
			}
		}

		view.setCenter(view.getCenter() + (newView.getCenter() - view.getCenter()) * viewInterpolateRate * dt);
		view.setSize(view.getSize() + (newView.getSize() - view.getSize()) * viewInterpolateRate * dt);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i dPos = sf::Mouse::getPosition() - prevMousePos;

			view.setCenter(view.getCenter() - sf::Vector2f(dPos.x * (view.getSize().x / window.getSize().x), dPos.y * (view.getSize().x / window.getSize().x)));

			newView.setCenter(view.getCenter());
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::Vector2f mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));

			sf::Vector2i gridCoords(mouseCoords.x / world.getCellWidth(), mouseCoords.y / world.getCellHeight());

			if (gridCoords.x >= 0 && gridCoords.y >= 0 && gridCoords.x < world.getWidth() && gridCoords.y < world.getHeight()) {
				bool hasObstacle = false;
				
				for (std::set<int>::const_iterator cit = world.getCell(gridCoords)._occupantIds.begin(); cit != world.getCell(gridCoords)._occupantIds.end(); cit++) {
					if (world.getGridObject(*cit)->getName() == "obstacle") {
						hasObstacle = true;

						break;
					}
				}

				if (!hasObstacle) {
					Ptr<ants::Obstacle> obstacle = make<ants::Obstacle>();

					world.add(obstacle, gridCoords);
				}
			}
		}

		prevMousePos = sf::Mouse::getPosition();

		window.clear();

		window.setView(view);

		updateTimer += dt;

		if (updateTimer > updateTime) {
			world.update();
			updateTimer -= updateTime;
		}

		world.render(window);

		window.display();

		dt = clock.getElapsedTime().asSeconds();

		clock.restart();
	} while (!quit);

	return 0;
}

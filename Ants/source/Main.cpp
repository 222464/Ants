#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <ants/World.h>
#include <ants/objects/TestAnt.h>

int main() {
	sf::RenderWindow window;
	window.create(sf::VideoMode(1280, 800), "ALife");

	window.setVerticalSyncEnabled(true);
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

	world.create(64, 64, colors, gridTexture);

	Ptr<ants::TestAnt> testAnt = make<ants::TestAnt>();

	world.add(testAnt, sf::Vector2i(32, 32));

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

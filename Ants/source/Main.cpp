#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main() {
	sf::RenderWindow window;
	window.create(sf::VideoMode(1280, 800), "ALife");

	window.setVerticalSyncEnabled(true);

	sf::Clock clock;

	float dt = 0.017f;

	sf::View view;
	sf::View newView;

	view.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	view.setCenter(sf::Vector2f(worldObjectGrid->getWidth() * 0.5f * worldObjectGrid->_cellSize.x, worldObjectGrid->getHeight() * 0.5f * worldObjectGrid->_cellSize.y));

	newView = view;

	float zoomRate = 0.2f;

	float viewInterpolateRate = 20.0f;

	sf::Vector2i prevMousePos = sf::Mouse::getPosition();

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

		world.render(window);

		window.display();

		//dt = clock.getElapsedTime().asSeconds();
		clock.restart();
	} while (!quit);

	return 0;
}

#include<SFML/Graphics.hpp>
#include<iostream>

int main() {

	const int screenwidth = 800;
	const int screenheight = 600;
	sf::RenderWindow window(sf::VideoMode(screenwidth, screenheight), "Rectangle");

	float length = 200.f;
	float width = 100.f;
	sf::RectangleShape rectangle(sf::Vector2f(length , width));

	const int velocity = 5;
	

	rectangle.setFillColor(sf::Color::Cyan);
	rectangle.setPosition(300.f, 250.f);

	while (window.isOpen()) {

		sf::Event event;

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) rectangle.move(-velocity, 0.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rectangle.move(velocity, 0.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) rectangle.move(0.f, -velocity);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) rectangle.move(0.f, velocity);

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					rectangle.setPosition(event.mouseButton.x, event.mouseButton.y);
			}

		}

		window.clear();
		window.draw(rectangle);
		window.display();
	}
	window.close();

	return 0;
}


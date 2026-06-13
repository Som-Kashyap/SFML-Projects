#include<SFML/Graphics.hpp>
#include<iostream>
#include<cstdlib>
#include<ctime>

int main() {

	float screenWidth = 800.f;
	float screenHeight = 600.f;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Auto Moving Ball");

	float radius = 20.f;	
	sf::CircleShape ball(20.f);
	ball.setFillColor(sf::Color::Red);
	ball.setPosition(400.f, 300.f);

	float deltaTime = 0.f;
	sf::Clock clock;

	sf::Vector2f velocity(500.0f, 400.0f);

	while (window.isOpen())
	{
		sf::Event event;

		deltaTime = clock.restart().asSeconds();

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {

				window.close();

			}

		}

		ball.move(velocity * deltaTime);

		if (ball.getPosition().x + 2 * radius >= screenWidth || ball.getPosition().x <= 0) {

			velocity.x = -velocity.x;
			ball.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));

		}
		if (ball.getPosition().y + 2 * radius >= screenHeight || ball.getPosition().y <= 0) {

			velocity.y = -velocity.y;
			ball.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));

		}


		window.clear();
		window.draw(ball);
		window.display();

	}

	window.close();

	return 0;
}


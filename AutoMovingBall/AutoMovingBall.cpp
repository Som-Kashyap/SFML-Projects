#include<SFML/Graphics.hpp>
#include<iostream>

int main() {

	float screenWidth = 800.f;
	float screenHeight = 600.f;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Auto Moving Ball");

	float radius = 20.f;	
	sf::CircleShape ball(20.f);
	ball.setFillColor(sf::Color::Red);
	ball.setPosition(400.f, 300.f);

	sf::RectangleShape paddle(sf::Vector2f(100.f, 20.f));
	paddle.setFillColor(sf::Color::Blue);
	paddle.setPosition(350.f, 570.f);
	float paddleSpeed = 1.f;

	sf::Vector2f velocity(1.0f, 1.0f);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

		}

		ball.move(velocity);
		if (ball.getPosition().x + 2 * radius >= screenWidth || ball.getPosition().x <= 0) {
			velocity.x = -velocity.x;
		}
		if (ball.getPosition().y + 2 * radius >= screenHeight || ball.getPosition().y <= 0) {
			velocity.y = -velocity.y;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			paddle.move(-paddleSpeed, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			paddle.move(paddleSpeed, 0.f);
		}

		if(ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
			velocity.y = -velocity.y;
		}


		window.clear();
		window.draw(ball);
		window.draw(paddle);
		window.display();

	}

	window.close();

	return 0;
}


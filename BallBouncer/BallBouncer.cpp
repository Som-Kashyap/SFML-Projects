#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Ball Bouncer");

    window.setFramerateLimit(60);

	// Creating ball
    sf::CircleShape ball(20.f);
    ball.setFillColor(sf::Color::Red);
    ball.setPosition(400.f, 300.f);

    sf::Vector2f ballVelocity(5.f, 3.f);

	// Maximum speed for the ball
	float maxspeed = 10.f;

    float paddleWidth = 100.f;
    float paddleHeight = 20.f;

	// Creating paddle
    sf::RectangleShape paddle(sf::Vector2f(paddleWidth, paddleHeight));
    paddle.setFillColor(sf::Color::Blue);
    // Place paddle centered horizontally near the bottom
    paddle.setPosition((windowWidth - paddleWidth) / 2.f, windowHeight - paddleHeight - 10.f);

	// Paddle movement speed
    float paddleSpeed = 5.0f;

	//loading font for text display
	sf::Font font;
    if (!font.loadFromFile("resources/arial.ttf")) {
        std::cout << "FONT FAILED!\n";
    }
		
	sf::Text text;
    text.setFont(font);
	text.setString("Use left and right arrow keys to move the paddle");
    text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);
	text.setPosition(10.f, 10.f);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Input handling (outside event loop)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            // Move right but prevent leaving the window
            if (paddle.getPosition().x + paddleWidth < static_cast<float>(windowWidth)) {
                paddle.move(paddleSpeed, 0.f);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (paddle.getPosition().x > 0.f) {
                paddle.move(-paddleSpeed, 0.f);
            }
        }

        // (Optional) simple ball movement and bounce from window edges
        ball.move(ballVelocity);
        sf::Vector2f pos = ball.getPosition();
        float radius = ball.getRadius();
        if (pos.x <= 0.f || pos.x + radius * 2.f >= static_cast<float>(windowWidth)) {
            ballVelocity.x = -ballVelocity.x;
        }
        if (pos.y <= 0.f || pos.y + radius * 2.f >= static_cast<float>(windowHeight)) {
			
            ballVelocity.y = -ballVelocity.y;
        }

		// Check collision with paddle
		if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            ballVelocity.y = -1.1*ballVelocity.y; // Simple bounce effect

			if (abs(ballVelocity.y) > maxspeed) {
                
				ballVelocity.y = maxspeed * (ballVelocity.y > 0 ? 1 : -1); // Cap the speed and maintain direction
            }
        }

        window.clear(sf::Color::Black);
        window.draw(ball);
        window.draw(paddle);
		window.draw(text);
        window.display();
    }

    return 0;
}
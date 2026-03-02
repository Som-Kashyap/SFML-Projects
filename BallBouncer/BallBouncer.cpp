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
    //ball.setPosition(400.f, 300.f);

    sf::Vector2f ballVelocity(5.f, 3.f);

	// Maximum speed for the ball
	float maxspeed = 10.f;

    float paddleWidth = 100.f;
    float paddleHeight = 20.f;

	// Creating paddle
    sf::RectangleShape paddle(sf::Vector2f(paddleWidth, paddleHeight));
    paddle.setFillColor(sf::Color::Yellow);
    // Place paddle centered horizontally near the bottom
    paddle.setPosition((windowWidth - paddleWidth) / 2.f, windowHeight - paddleHeight - 10.f);

	// Paddle movement speed
    float paddleSpeed = 5.0f;

	//loading font for text display
	sf::Font font;
    bool showText = true;
    if (!font.loadFromFile("resources/arial.ttf")) {
        std::cout << "FONT FAILED!\n";
    }

	sf::Font font2;
    if (!font2.loadFromFile("resources/ARIALNB.ttf")) {
        std::cout << "FONT FAILED!\n";
	}

	//creating score variable and text to display it
	int score = 0;
		
	sf::Text text;
    text.setFont(font);
	text.setString("Use left and right arrow keys to move the paddle");
    text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);
	text.setPosition(10.f, 10.f);

	sf::Text text2;
	bool showText2 = true;
	text2.setFont(font);
    text2.setString("(Click the mouse to hide text!)");
	text2.setCharacterSize(20);
	text2.setFillColor(sf::Color::Red);
    text2.setPosition(10.f, 40.f);

	// Game state variables
	bool moveBall = false;

    sf::Text name;
	bool showName = true;
	name.setFont(font2);
	name.setString("BALL BOUNCER");
	name.setCharacterSize(50);
    name.setFillColor(sf::Color::Color(255, 165, 0));
	name.setPosition(200.f, 200.f);

    sf::Text start;
	bool showStart = true;
	start.setFont(font2);
    start.setString("Hit Enter to Start!");
	start.setCharacterSize(50);
    start.setFillColor(sf::Color::Green);
	start.setPosition(150.f, 300.f);

	sf::Text scoreText;
	scoreText.setFont(font);
	//scoreText.setString("Score: " + std::to_string(score));
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::Green);
	scoreText.setPosition(10.f, 70.f);

    sf::Text gameOver;
	bool isGameOver = false;
	gameOver.setFont(font2);
	gameOver.setString("GAME OVER!");
	gameOver.setCharacterSize(50);
	gameOver.setFillColor(sf::Color::Red);
	gameOver.setPosition(250.f, 300.f);


    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                showStart = false;
				moveBall = true;
                ballVelocity = { 5.0f, 3.0f }; // Set initial velocity when the game starts
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    showText = false; // Hide text on mouse click
                    showText2 = false;
                }
            }
        }


        if(moveBall) {

            ball.move(ballVelocity);

            sf::Vector2f pos = ball.getPosition();
            float radius = ball.getRadius();

            if (pos.x <= 0.f || pos.x + radius * 2.f >= static_cast<float>(windowWidth)) {
                ballVelocity.x = -ballVelocity.x;
            }
            if (pos.y <= 0.f) {
                ballVelocity.y = -ballVelocity.y;

                //ballVelocity.y = -ballVelocity.y;
            }

            if (pos.y + radius * 2.f >= static_cast<float>(windowHeight)) {
                isGameOver = true;
                showName = false;

            }



            // Check collision with paddle
            if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
                score += 10; // Increment score on paddle hit
                scoreText.setString("Score: " + std::to_string(score)); // Update score display

                ballVelocity.y = -1.1 * ballVelocity.y; // Simple bounce effect

                if (abs(ballVelocity.y) > maxspeed) {

                    ballVelocity.y = maxspeed * (ballVelocity.y > 0 ? 1 : -1); // Cap the speed and maintain direction
                }
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
        

        // simple ball movement and bounce from window edges


       

        window.clear(sf::Color::Blue);
        window.draw(ball);
        window.draw(paddle);

		if (showText) {
            window.draw(text);
		}
		if (showText2) {
			window.draw(text2);
		}
		window.draw(scoreText);
		if (isGameOver) {
            window.draw(gameOver);
		}
		if (showName) {
			window.draw(name);
		}
		if (showStart) {
			window.draw(start);
		}
        window.display();
    }

    return 0;
}
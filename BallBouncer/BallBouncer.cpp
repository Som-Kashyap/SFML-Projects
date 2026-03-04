#include <SFML/Graphics.hpp>
#include <iostream>

enum class GameState
{
    StartScreen,
    Playing,
    Paused,
    HelpScreen,
    GameOver
};

class Game {
public:
    Game();
    void run();

private:
	void handleEvents();
	void update();
    void render();
	void resetGame();

private:
	const unsigned int windowWidth = 800;
	const unsigned int windowHeight = 600;

	sf::RenderWindow window;

	sf::CircleShape ball;
	sf::Vector2f ballVelocity;
    float maxspeed;

	sf::RectangleShape paddle;
	float paddleSpeed;
	float paddleWidth;
	float paddleHeight;

	sf::Font font;
	sf::Font font2;

    int score;

	sf::Text name;
	sf::Text start;
	sf::Text scoreText;
	sf::Text gameOver;
	sf::Text restart;
	sf::Text finalScore;
	sf::Text pausedText;
	sf::Text resumeText;
	sf::Text helpText;
	sf::Text showHelp;
	sf::Text menuText;

	GameState gameState;
};

Game::Game() : window(sf::VideoMode(windowWidth, windowHeight), "Ball Bouncer")
{
	window.setFramerateLimit(60);

	ball.setRadius(20.f);
	ball.setFillColor(sf::Color::Red);

	ballVelocity = { 5.f, 3.f };
	maxspeed = 10.f;

	paddleWidth = 100.f;
	paddleHeight = 20.f;
	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	paddle.setFillColor(sf::Color::Yellow);
	paddle.setPosition((windowWidth - paddleWidth) / 2.f, windowHeight - paddleHeight - 10.f);
	paddleSpeed = 5.0f;

    if (!font.loadFromFile("resources/arial.ttf")) {
        std::cout << "FONT FAILED!\n";
	}
    if (!font2.loadFromFile("resources/ARIALNB.ttf")) {
        std::cout << "FONT FAILED!\n";
	}

    score = 0;

	name.setFont(font2);
	name.setString("BALL BOUNCER");
	name.setCharacterSize(50);
	name.setFillColor(sf::Color::Color(255, 165, 0));
	name.setPosition(200.f, 200.f);

	start.setFont(font2);
	start.setString("Hit Enter to Start!");
	start.setCharacterSize(50);
	start.setFillColor(sf::Color::Green);
	start.setPosition(200.f, 300.f);

	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::Green);
	scoreText.setPosition(10.f, 70.f);

	gameOver.setFont(font2);
	gameOver.setString("GAME OVER!");
	gameOver.setCharacterSize(50);
	gameOver.setFillColor(sf::Color::Red);
	gameOver.setPosition(250.f, 300.f);

	restart.setFont(font);
	restart.setString("Hit Enter to restart");
	restart.setCharacterSize(30);
	restart.setFillColor(sf::Color::Magenta);
	restart.setPosition(250.f, 400.f);

	finalScore.setFont(font2);
	finalScore.setCharacterSize(30);
	finalScore.setFillColor(sf::Color::Yellow);
	finalScore.setPosition(250.f, 50.f);

	pausedText.setFont(font2);
	pausedText.setString("PAUSED");
	pausedText.setCharacterSize(50);
	pausedText.setFillColor(sf::Color::White);
	pausedText.setPosition(300.f, 300.f);

	resumeText.setFont(font);
	resumeText.setString("Press P to Resume");
	resumeText.setCharacterSize(30);
	resumeText.setFillColor(sf::Color::Green);
	resumeText.setPosition(250.f, 0.f);

	helpText.setFont(font);
	helpText.setString("Use left and right arrow keys to move the paddle. Don't let the ball fall!");
	helpText.setCharacterSize(20);
	helpText.setFillColor(sf::Color::Red);
	helpText.setPosition(10.f, 10.f);

	showHelp.setFont(font);
	showHelp.setString("Press H for Help");
	showHelp.setCharacterSize(20);
	showHelp.setFillColor(sf::Color::White);
	showHelp.setPosition(200.f, 400.f);

	menuText.setFont(font);
	menuText.setString("Press 0 to return to Menu");
	menuText.setCharacterSize(20);
	menuText.setFillColor(sf::Color::White);
	menuText.setPosition(250.f, 500.f);

	gameState = GameState::StartScreen;

}
void Game:: handleEvents() {
    sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                //showStart = false;
                //moveBall = true;
                //movePaddle = true;

                if (gameState == GameState::StartScreen) {

                    gameState = GameState::Playing;

                    ball.setPosition(400.f, 300.f);
                    ballVelocity = { 5.0f , 3.0f };
                    score = 0;


                }
                else if (gameState == GameState::GameOver) {

                    gameState = GameState::Playing;
                    paddle.setPosition((windowWidth - paddleWidth) / 2.f, windowHeight - paddleHeight - 10.f);
                    ball.setPosition(400.f, 300.f);
                    ballVelocity = { 5.0f , 3.0f };
                    score = 0;

                }
            }


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                if (gameState == GameState::Playing) {
                    gameState = GameState::Paused;
                }
                else if (gameState == GameState::Paused) {
                    gameState = GameState::Playing;
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H) {
                if (gameState == GameState::HelpScreen) {
                    gameState = GameState::StartScreen;
                }
                else if (gameState == GameState::StartScreen) {
                    gameState = GameState::HelpScreen;
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num0) {
                gameState = GameState::StartScreen;
            }
        }

    }

void Game :: update() {
    if (gameState == GameState::Playing) {
        // Move the ball
        ball.move(ballVelocity);
        // Check for collisions with the window borders
        if (ball.getPosition().x <= 0 || ball.getPosition().x + ball.getRadius() * 2 >= windowWidth) {
            ballVelocity.x = -ballVelocity.x;
        }
        if (ball.getPosition().y <= 0) {
            ballVelocity.y = -ballVelocity.y;
        }
        // Check for collision with the paddle
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            ballVelocity.y = -ballVelocity.y;
            score++;
            // Increase speed slightly on each hit, up to maxspeed
            if (std::abs(ballVelocity.x) < maxspeed) {
                ballVelocity.x *= 1.05f;
            }
            if (std::abs(ballVelocity.y) < maxspeed) {
                ballVelocity.y *= 1.05f;
            }
        }
        // Check for game over condition
        if (ball.getPosition().y > windowHeight) {
            gameState = GameState::GameOver;
        }
        // Move the paddle
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            paddle.move(-paddleSpeed, 0);
            if (paddle.getPosition().x < 0) {
                paddle.setPosition(0, paddle.getPosition().y);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            paddle.move(paddleSpeed, 0);
            if (paddle.getPosition().x + paddleWidth > windowWidth) {
                paddle.setPosition(windowWidth - paddleWidth, paddle.getPosition().y);
            }
        }
        scoreText.setString("Score: " + std::to_string(score));
    }
}

void Game:: render() {
    window.clear(sf::Color::Blue);
    if (gameState == GameState::StartScreen) {
        window.draw(name);
        window.draw(start);
        window.draw(showHelp);
    }
    else if (gameState == GameState::Playing) {
        window.draw(ball);
        window.draw(paddle);
        window.draw(scoreText);
    }
    else if (gameState == GameState::Paused) {
        window.draw(pausedText);
        window.draw(resumeText);
    }
    else if (gameState == GameState::HelpScreen) {
        window.draw(helpText);
    }
    else if (gameState == GameState::GameOver) {
        window.draw(gameOver);
        finalScore.setString("Final Score: " + std::to_string(score));
        window.draw(finalScore);
        window.draw(restart);
        window.draw(menuText);
    }
    window.display();
}

void Game::resetGame() {
    ball.setPosition(400.f, 300.f);
    ballVelocity = { 5.f, 3.f };
    paddle.setPosition((windowWidth - paddleWidth) / 2.f, windowHeight - paddleHeight - 10.f);
    score = 0;
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}
int main() {
    
	Game game;
	game.run();

    return 0;
}
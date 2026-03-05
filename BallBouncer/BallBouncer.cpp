#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

enum class GameState
{
    StartScreen,
    Playing,
    Paused,
    HelpScreen,
    GameOver,
    Victory
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
	void UI();
	void SoundSetup();
	

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

	sf::RectangleShape opponentPaddle;
	float opponentPaddleSpeed;

	sf::Font font;
	sf::Font font2;

    int score;
	int opponentScore;

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
    sf::Text timeText;
	sf::Text opponentGameOver;
	sf::Text opponentScoreText;

	sf::SoundBuffer bounceBuffer;
	sf::Sound bounceSound;

	sf::SoundBuffer gameOverBuffer;
    sf::Sound gameOverSound;

    sf::SoundBuffer clickBuffer;
	sf::Sound clickSound;

    sf::Music bgm;

    sf::Clock clock;
    sf::Clock deltaTimeClock;
	float deltaTime;
   
	sf::Texture bgtexture;
	sf::Sprite background;
    
	GameState gameState;
};

Game::Game() : window(sf::VideoMode(windowWidth, windowHeight), "Ball Bouncer")
{
	window.setFramerateLimit(60);

	ball.setRadius(20.f);
	ball.setFillColor(sf::Color::Red);

	ballVelocity = { 500.f, -300.f };
	maxspeed = 600.f;

	paddleWidth = 100.f;
	paddleHeight = 20.f;
	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	paddle.setFillColor(sf::Color::Yellow);
	paddle.setPosition((windowWidth - paddleWidth) / 2.f, windowHeight - paddleHeight - 10.f);
	paddleSpeed = 600.0f;

	opponentPaddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	opponentPaddle.setFillColor(sf::Color::Green);
	opponentPaddle.setPosition((windowWidth - paddleWidth) / 2.f, 10.f);
	opponentPaddleSpeed = 600.0f;

    score = 0;
	opponentScore = 0;
  
	deltaTime = 0.f;


    if (!bgtexture.loadFromFile("resources/background.png")) {
        std::cout << "TEXTURE FAILED!\n";
    }

    UI();
    SoundSetup();

    background.setTexture(bgtexture);

    background.setScale(window.getSize().x / (float)bgtexture.getSize().x, (float)window.getSize().y / bgtexture.getSize().y);

    gameState = GameState::StartScreen;

}

void Game::UI() {
   

    if (!font.loadFromFile("resources/arial.ttf")) {
        std::cout << "FONT FAILED!\n";
    }
    if (!font2.loadFromFile("resources/ARIALNB.ttf")) {
        std::cout << "FONT FAILED!\n";
    }

   

    name.setFont(font2);
    name.setString("BALL BOUNCER");
    name.setCharacterSize(50);
    name.setFillColor(sf::Color::Color(255, 165, 0));
    name.setPosition(200.f, 200.f);
    name.setStyle(sf::Text::Bold);
    name.setOutlineColor(sf::Color::Black);
    name.setOutlineThickness(5.f);

    start.setFont(font2);
    start.setString("Hit Enter to Start!");
    start.setCharacterSize(50);
    start.setFillColor(sf::Color::Green);
    start.setPosition(200.f, 300.f);
    start.setStyle(sf::Text::Bold);
    start.setOutlineColor(sf::Color::Black);
    start.setOutlineThickness(2.f);

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Green);
    scoreText.setPosition(10.f, 70.f);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2.f);

    gameOver.setFont(font2);
    gameOver.setString("GAME OVER!");
    gameOver.setCharacterSize(50);
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setPosition(250.f, 300.f);
    gameOver.setStyle(sf::Text::Bold);
    gameOver.setOutlineColor(sf::Color::Black);
    gameOver.setOutlineThickness(5.f);

    restart.setFont(font);
    restart.setString("Hit Enter to restart");
    restart.setCharacterSize(30);
    restart.setFillColor(sf::Color::Magenta);
    restart.setPosition(250.f, 400.f);
    restart.setOutlineColor(sf::Color::Black);
    restart.setOutlineThickness(2.f);

    finalScore.setFont(font2);
    finalScore.setString("Final Score: " + std::to_string(score));
    finalScore.setCharacterSize(50);
    finalScore.setFillColor(sf::Color::Yellow);
    finalScore.setPosition(250.f, 50.f);
    finalScore.setStyle(sf::Text::Bold);

    finalScore.setOutlineColor(sf::Color::Black);
    finalScore.setOutlineThickness(2.f);

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
    resumeText.setOutlineColor(sf::Color::Black);
    resumeText.setOutlineThickness(2.f);

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
    showHelp.setOutlineColor(sf::Color::Black);
    showHelp.setOutlineThickness(2.f);

    menuText.setFont(font);
    menuText.setString("Press 0 to return to Menu");
    menuText.setCharacterSize(20);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition(250.f, 500.f);
    menuText.setOutlineColor(sf::Color::Black);
    menuText.setOutlineThickness(2.f);

    timeText.setFont(font);
    timeText.setCharacterSize(20);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(700.f, 10.f);
    timeText.setOutlineColor(sf::Color::Black);
    timeText.setOutlineThickness(2.f);

	opponentGameOver.setFont(font2);
	opponentGameOver.setString("YOU WON!");
	opponentGameOver.setCharacterSize(50);
	opponentGameOver.setFillColor(sf::Color::Green);
	opponentGameOver.setPosition(250.f, 300.f);
	opponentGameOver.setStyle(sf::Text::Bold);

	opponentScoreText.setFont(font);
	opponentScoreText.setCharacterSize(20);
	opponentScoreText.setFillColor(sf::Color::Black);
	opponentScoreText.setPosition(600.f, 70.f);
	opponentScoreText.setOutlineColor(sf::Color::Black);
	opponentGameOver.setOutlineThickness(2.f);


}
void Game::SoundSetup() {


    if (!bounceBuffer.loadFromFile("resources/BallCollision.wav")) {
        std::cout << "SOUND FAILED!\n";
    }

    if (!gameOverBuffer.loadFromFile("resources/GameOver.wav")) {
        std::cout << "SOUND FAILED!\n";
    }

    if (!bgm.openFromFile("resources/bgm.wav")) {
        std::cout << "MUSIC FAILED!\n";
    }
    if (!clickBuffer.loadFromFile("resources/click.wav")) {
        std::cout << "SOUND FAILED!\n";
    }  
    bgm.setLoop(true);
    bgm.setVolume(50.f);

    bounceSound.setBuffer(bounceBuffer);

    bounceSound.setVolume(100.f);

    gameOverSound.setBuffer(gameOverBuffer);
    gameOverSound.setVolume(100.f);

    clickSound.setBuffer(clickBuffer);
    clickSound.setVolume(100.f);
}
void Game::handleEvents() {

    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {

            clock.restart(); // Reset the clock for the next game
            if (gameState == GameState::StartScreen) {

                gameState = GameState::Playing;
                bgm.play();
                clickSound.play();

                ball.setPosition(400.f, 300.f);
                ballVelocity = { 500.0f , -300.0f };
                score = 0;


            }
            else if (gameState == GameState::GameOver) {

                gameState = GameState::Playing;
                bgm.play();
                clickSound.play();
                resetGame();

            }
            else if (gameState == GameState::Victory) {
                gameState = GameState::Playing;
                bgm.play();
                clickSound.play();
				resetGame();
            }


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                if (gameState == GameState::Playing) {
                    gameState = GameState::Paused;
                    bgm.pause();
                    clickSound.play();
                }
                else if (gameState == GameState::Paused) {
                    gameState = GameState::Playing;
                    bgm.play();
                    clickSound.play();
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H) {
                if (gameState == GameState::HelpScreen) {
                    gameState = GameState::StartScreen;
                    clickSound.play();
                }
                else if (gameState == GameState::StartScreen) {
                    gameState = GameState::HelpScreen;
                    clickSound.play();
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num0) {
                if (gameState == GameState::GameOver || gameState == GameState::HelpScreen || gameState == GameState::Victory) {
                    gameState = GameState::StartScreen;
                    clickSound.play();
                }

            }

            float elapsed = clock.getElapsedTime().asSeconds();

        }

    }
}

void Game::update() {

    if (gameState == GameState::Playing) {
        // Move the ball

        ball.move(ballVelocity * deltaTime);

        // Check for collisions with the window borders
        if (ball.getPosition().x <= 0 || ball.getPosition().x + ball.getRadius() * 2 >= windowWidth) {
            ballVelocity.x = -ballVelocity.x;
        }
        if (ball.getPosition().y <= 0) {
            ballVelocity.y = -ballVelocity.y;
        }

        // Check for collision with the paddle

        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()) && ballVelocity.y > 0) {

            float ballCenterX = ball.getPosition().x + ball.getRadius();
            float paddleCenterX = paddle.getPosition().x + paddleWidth / 2.f;
            float distanceFromCenter = ballCenterX - paddleCenterX;
            float normalise = distanceFromCenter / (paddleWidth / 2.f);

            float speed = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
            if (speed > maxspeed) {
                ballVelocity.x = (ballVelocity.x / speed) * maxspeed;
                ballVelocity.y = (ballVelocity.y / speed) * maxspeed;
            }
            float angle = normalise * 75.f; // Max angle of 75 degrees
            float radians = angle * 3.14159f / 180.f;
            ballVelocity.x = speed * std::sin(radians);
            ballVelocity.y = -std::abs(speed * std::cos(radians)); // Always bounce upwards

            float paddletop = paddle.getPosition().y;
            float ballHeight = ball.getPosition().y + ball.getRadius() * 2;

            ball.setPosition(ball.getPosition().x, paddletop - ball.getRadius() * 2 - 1);// Position the ball just above the paddle

            bounceSound.play();

            score += 10;

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
            gameOverSound.play();

        }
        // Move the paddle
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            paddle.move(-paddleSpeed * deltaTime, 0);
            if (paddle.getPosition().x < 0) {
                paddle.setPosition(0, paddle.getPosition().y);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            paddle.move(paddleSpeed * deltaTime, 0);
            if (paddle.getPosition().x + paddleWidth > windowWidth) {
                paddle.setPosition(windowWidth - paddleWidth, paddle.getPosition().y);
            }
        }
        scoreText.setString("Score: " + std::to_string(score));

        if (ballVelocity.y < 0) { // Only move the opponent paddle when the ball is moving towards it
            if (ball.getPosition().x + ball.getRadius() < opponentPaddle.getPosition().x + paddleWidth / 2.f) {
                opponentPaddle.move(-opponentPaddleSpeed * deltaTime, 0);
                if (opponentPaddle.getPosition().x < 0) {
                    opponentPaddle.setPosition(0, opponentPaddle.getPosition().y);
                }
            }
            else if (ball.getPosition().x + ball.getRadius() > opponentPaddle.getPosition().x + paddleWidth / 2.f) {
                opponentPaddle.move(opponentPaddleSpeed * deltaTime, 0);
                if (opponentPaddle.getPosition().x + paddleWidth > windowWidth) {
                    opponentPaddle.setPosition(windowWidth - paddleWidth, opponentPaddle.getPosition().y);
                }
            }
            if (opponentPaddle.getGlobalBounds().intersects(ball.getGlobalBounds())) {
                ballVelocity.y = -ballVelocity.y;
                opponentScore += 10;
                opponentScoreText.setString("Opponent Score: " + std::to_string(opponentScore));
                bounceSound.play();
            }
            

        }
            if (ball.getPosition().y <= opponentPaddle.getPosition().y + paddleHeight && ballVelocity.y < 0) {

                gameState = GameState::Victory;
                gameOverSound.play();
            }

    }
}

void Game:: render() {

	
    window.clear(sf::Color::Blue);
    window.draw(background);
    if (gameState == GameState::StartScreen) {
        window.draw(name);
        window.draw(start);
        window.draw(showHelp);
    }
    else if (gameState == GameState::Playing) {
        window.draw(ball);
        window.draw(paddle);
		window.draw(opponentPaddle);
        window.draw(scoreText);
		window.draw(opponentScoreText);
		timeText.setString("Time: " + std::to_string(static_cast<int>(clock.getElapsedTime().asSeconds())) + "s");
		window.draw(timeText);
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
    if (gameState == GameState::Victory) {
        window.draw(opponentGameOver);
        finalScore.setString("Final Score: " + std::to_string(score));
        window.draw(finalScore);
        window.draw(restart);
        window.draw(menuText);
    }

    window.display();

}

void Game::resetGame() {

    ball.setPosition(400.f, 300.f);
    ballVelocity = { 500.f, -300.f };
    paddle.setPosition((windowWidth - paddleWidth) / 2.f, windowHeight - paddleHeight - 10.f);
    score = 0;
    bgm.play();

	opponentPaddle.setPosition((windowWidth - paddleWidth) / 2.f, 10.f);
    opponentScore = 0;

}

void Game::run() {

    while (window.isOpen()) {

        deltaTime = deltaTimeClock.restart().asSeconds(); //time in seconds since last frame
       
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
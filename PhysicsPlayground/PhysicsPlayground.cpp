#include<SFML/Graphics.hpp>
#include<iostream>

enum gameState{

	Start,
	Playing,
	ReLaunchBall,

};

class Game {

private:
	sf::RenderWindow window;
	const int windowWidth = 800;
	const int windowHeight = 600;
	sf::CircleShape ball;
	sf::Vector2f velocity;

	sf::Time deltaTime;

	gameState GameState;

private:

	void HandleEvents();
	void Update();
	void Render();
	//void textSetup();
	//void AudioSetup();
	

public:
	Game();
	void RunGame();
	
};

Game :: Game() : window(sf::VideoMode(800, 600), "Physics Playground") {

	ball.setRadius(20.f);
	ball.setFillColor(sf::Color::Red);
	ball.setPosition(400.f, 300.f);

	velocity = sf::Vector2f(5.f, 4.f);

	GameState=gameState::Start;

	//textSetup();
	//AudioSetup();
}

void Game :: HandleEvents() {

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				window.close();
			}
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {

			if (GameState == gameState::Start) {

				GameState = gameState::Playing;
				
				}
	
			}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {

			if (GameState == gameState::Playing) {
				GameState = gameState::ReLaunchBall;

				velocity = sf::Vector2f(5.f, 4.f);
				velocity.x = -velocity.x;
				Update();
			}
			else if (GameState == gameState::ReLaunchBall) {
				velocity = sf::Vector2f(-40.f, 100.f);
				Update();
			}

		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::RControl) {
			if (GameState == gameState::Playing) {
				GameState = gameState::ReLaunchBall;
				velocity = sf::Vector2f(5.f, 4.f);
				Update();
			}
			else if (GameState == gameState::ReLaunchBall) {
				velocity = sf::Vector2f(40.f, 100.f);
				Update();
			}

		}
		
		
	}
}
void Game :: Update() {

	if (GameState == gameState::ReLaunchBall) {

		//velocity.x *= 0.99f; // Simulate friction

		const float deltaTime = 1.f / 60.f;

		velocity.y += 9.f * deltaTime;
		ball.move(velocity * deltaTime);

		if (ball.getPosition().x <= 0 || ball.getPosition().x + ball.getRadius() * 2 >= windowWidth) {

			velocity.x = -velocity.x*0.9;
		}
		if (ball.getPosition().y <= 0 || ball.getPosition().y + ball.getRadius()*2 >= windowHeight ){
			ball.setPosition(ball.getPosition().x, windowHeight - (ball.getRadius() * 2)-0.1);
			velocity.y = -velocity.y*0.9;
		}
	}

}
void Game :: Render() {
	window.clear();

	if (GameState == gameState::Playing || GameState == gameState::ReLaunchBall ) {
		window.draw(ball);
	}
	window.display();
}
void Game::RunGame(){
	while (window.isOpen()) {
		HandleEvents();
		Update();
		Render();
	}
}
int main() {
	Game game;

	game.RunGame();

	return 0;
}
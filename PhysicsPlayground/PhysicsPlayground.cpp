#include<SFML/Graphics.hpp>
#include<ctime>
#include<cstdlib>
#include<iostream>

enum class GameState {

	StartScreen,
	Playing,
	Paused
};

class Ball {

public:
	Ball(sf::Vector2f);
	const float ballradius = 20.f;
	sf::CircleShape shape;
	sf::Vector2f ballvelocity;
	void Update(float deltaTime);
private:


};

class Game {

public:
	Game();
	std::vector<Ball>balls;
	void rungame();
private:
	void update(float deltaTime);
	void HandleEvents();
	void render();
	sf::CircleShape circle;

 sf::Clock deltaTimeclock;
	 float deltaTime;
	
	 const int windowWidth = 800;
	 const int windowHeight = 600;
	  sf::RenderWindow window;
};


Game:: Game() : window(sf::VideoMode(windowWidth , windowHeight) , "Physics Playground")  {

	
	window.setFramerateLimit(60);

}

Ball::Ball(sf::Vector2f mousepos) {

	shape.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	shape.setRadius(ballradius);
	shape.setPosition(mousepos);
	ballvelocity.x = 0.f;
	ballvelocity.y = 500.f;

}
void Game::update(float deltaTime) {

	for (auto& ball : balls) {
		ball.Update(deltaTime);
	}
}

void Ball::Update(float deltaTime) {

		shape.move(ballvelocity*deltaTime);
		ballvelocity.y += 980 * deltaTime;
		
		if (shape.getPosition().x + (shape.getRadius()) * 2.0f >= 800 || shape.getPosition().x < 0) {
			ballvelocity = -ballvelocity * 0.9f;
		}
		if (shape.getPosition().y < 0 || shape.getPosition().y + (shape.getRadius()) * 2.0f >= 600) {
			ballvelocity = -ballvelocity * 0.9f;
		}
}

void Game:: HandleEvents() {

	sf::Event event;

	while (window.pollEvent(event)) {

		if (event.type==(sf::Event::Closed)) {
			window.close();
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				
				sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				balls.emplace_back(mousepos);
			}
		}
	}

}

void Game::render() {

	window.clear(sf::Color::White);

	for (auto& ball : balls) {
		window.draw(ball.shape);
	}
	window.display();
}

	void Game::rungame()
	{
		while (window.isOpen())
		{
			while (window.isOpen())
			{
				float dt = deltaTimeclock.restart().asSeconds();

				HandleEvents();
				update(dt);
				render();
			}

			window.display();
		}
	}


int main() {

	Game game;
	game.rungame();
}
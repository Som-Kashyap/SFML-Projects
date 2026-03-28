#include<SFML/Graphics.hpp>
#include<ctime>
#include<cstdlib>
#include<iostream>

class Particles{

private:
	void spawnParticles();
};

class Game {

public:
	
	const float windowWidth = 800.f, windowHeight = 600.f;
	sf::RenderWindow window;
	sf::RectangleShape cannon;
	sf::Vector2f cannonVelocity;

	const float width = 20.f, height = 80.f;
	void rungame();
	Game();
private:
	
	sf::Clock deltaTimeClock;
	float deltaTime = 0.f;
	void update();
	void handleEvents();
	void render();
	
};

Game::Game() :window(sf::VideoMode(windowWidth, windowHeight), "Particle Shooter") {
	
	window.setFramerateLimit(60);

	cannon.setSize(sf::Vector2f(20.f,80.f));
	 cannonVelocity= {500.f , 0.f };
	cannon.setFillColor(sf::Color::Green);
	cannon.setPosition(400.f, 400.f);
}
void Game::handleEvents() {

	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			cannon.move(cannonVelocity*(-deltaTime));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			cannon.move(cannonVelocity * (deltaTime));
		}
	}
}
void Game::render() {

		window.clear();

		window.draw(cannon);

		window.display();
}
void Game::update() {

	if (cannon.getPosition().x + width > windowWidth) {
		cannon.setPosition(windowWidth - width, cannon.getPosition().y);
	}
	if (cannon.getPosition().x < 0) {
		cannon.setPosition(0, cannon.getPosition().y);
	}
}

void Game::rungame() {

	while (window.isOpen()) {
		deltaTime = deltaTimeClock.restart().asSeconds();
		handleEvents();
		update();
		render();

	}
}

int main() {
	Game game;
	game.rungame();

	return 0;
}
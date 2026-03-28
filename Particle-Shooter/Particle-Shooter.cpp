#include<SFML/Graphics.hpp>
#include<ctime>
#include<cstdlib>
#include<iostream>

class Particles{

public:
	sf::CircleShape bullet;
	sf::Vector2f bulletVelocity;
	void Update(float deltaTime);
	void spawnParticles();
};

class Enemy {
public:
	//Enemy();
	
	void createEnemy();
};

class Game {

public:
	
	const float windowWidth = 800.f, windowHeight = 600.f;
	sf::RenderWindow window;
	sf::RectangleShape cannon;
	sf::Vector2f cannonVelocity;
	std::vector<Particles>bullets;
	sf::Clock deltaTimeClock;
	float deltaTime = 0.f;

	const float enemyWidth = 40.f, enemyHeight = 120;
	sf::RectangleShape enemy;
	bool draw = true;

	const float width = 20.f, height = 80.f;
	void rungame();
	Game();
private:
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

	//enemy
	
	enemy.setFillColor(sf::Color::Red);
	enemy.setSize(sf::Vector2f(enemyWidth, enemyHeight));
	enemy.setPosition(300.f, 100.f);
}
void Particles::spawnParticles() {

	bulletVelocity = { 0,-500.0 };
	bullet.setFillColor(sf::Color::White);
	bullet.setRadius(5.0);

}

void Particles::Update(float deltaTime) {

	bullet.move(bulletVelocity * deltaTime);
}

//Enemy::Enemy(){

	

//}
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			Particles newBullet;
			newBullet.spawnParticles();
			newBullet.bullet.setPosition(cannon.getPosition().x + (width / 2.0), cannon.getPosition().y);
			bullets.emplace_back(newBullet);
		}

	}
}
void Game::render() {

		window.clear();

		if (draw == true) {
			window.draw(enemy);
		}
		window.draw(cannon);

		for (auto& val : bullets) {
			window.draw(val.bullet);
		}
		window.display();
}
void Game::update() {

	if (cannon.getPosition().x + width > windowWidth) {
		cannon.setPosition(windowWidth - width, cannon.getPosition().y);
	}
	if (cannon.getPosition().x < 0) {
		cannon.setPosition(0, cannon.getPosition().y);
	}

	for (auto& bullet : bullets) {
		bullet.Update(deltaTime);
	}
	for (auto& bullet : bullets) {
		if (bullet.bullet.getGlobalBounds().intersects(enemy.getGlobalBounds())) {
			draw = false;
		}
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
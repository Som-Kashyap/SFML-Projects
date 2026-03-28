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
	Enemy();
	sf::Vector2f enemyVelocity;
	sf::RectangleShape enemy;
	void createEnemy();
	void update(float deltaTime);
};

class Game {

public:
	
	const float windowWidth = 800.f, windowHeight = 600.f;
	sf::RenderWindow window;
	sf::RectangleShape cannon;
	sf::Vector2f cannonVelocity;
	std::vector<Enemy>enemies;
	const int enemyCount = 5;
	std::vector<Particles>bullets;
	sf::Clock deltaTimeClock;
	float deltaTime = 0.f;


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

	cannon.setSize(sf::Vector2f(20.f, 80.f));
	cannonVelocity = { 500.f , 0.f };
	cannon.setFillColor(sf::Color::Green);
	cannon.setPosition(400.f, 520.f);

	for(size_t i = 0 ;i < enemyCount; i++) {
		Enemy enemy;
		enemies.emplace_back(enemy);
	}

}
Enemy::Enemy() {

	 enemyVelocity = { 0 , 40.f };
	const float enemyWidth = 40.f, enemyHeight = 40.f;
	enemy.setSize(sf::Vector2f(enemyWidth, enemyHeight));
	enemy.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	enemy.setPosition(static_cast<float>(std::rand() %800 - static_cast<int>(enemyWidth - 1)) , static_cast<float>(std::rand()%-100+0));

}
void Particles::spawnParticles() {

	bulletVelocity = { 0,-500.0 };
	bullet.setFillColor(sf::Color::White);
	bullet.setRadius(5.0);

}

void Particles::Update(float deltaTime) {

	bullet.move(bulletVelocity * deltaTime);
}

void Enemy::update(float deltaTime) {

	enemy.move(enemyVelocity * deltaTime);

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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			Particles newBullet;
			newBullet.spawnParticles();
			newBullet.bullet.setPosition(cannon.getPosition().x + (width / 2.0), cannon.getPosition().y);
			bullets.emplace_back(newBullet);
		}

	}
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

	for (auto& enemy : enemies) {
		enemy.update(deltaTime);
	}
	for (size_t i = 0; i < enemies.size(); i++) {
		for (auto& bullet : bullets) {
			if (enemies[i].enemy.getGlobalBounds().intersects(bullet.bullet.getGlobalBounds())) {
				enemies.erase(enemies.begin() + i);
				i--; // adjust index after erase
				break;
			}
		}
	}
	bullets.erase(
		std::remove_if(bullets.begin(), bullets.end(),
			[](Particles& b) {
				return b.bullet.getPosition().y < 0;
			}),
		bullets.end()
	);

	if (enemies.size() == 0) {

		for (size_t i = 0; i < enemyCount; i++) {
			Enemy enemy;
			enemies.emplace_back(enemy);
		}
	}
}

void Game::render() {

		window.clear();

		window.draw(cannon);
	
		for (auto& val : bullets) {
			window.draw(val.bullet);
		}

		for (auto& enemy : enemies) {
			window.draw(enemy.enemy);
		}
		
		window.display();
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
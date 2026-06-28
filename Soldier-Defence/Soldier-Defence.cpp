#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>

class Bullets {

public:

	Bullets();
	sf::CircleShape bulletShape;
	sf::Vector2f bulletVelocity;

	void update(float deltaTime);

};

Bullets::Bullets() {

	float bulletRadius = 5.f;
	bulletShape.setRadius(bulletRadius);
	bulletVelocity = sf::Vector2f(800.f, 0.f);

}

void Bullets::update( float deltaTime) {
	bulletShape.move(bulletVelocity * deltaTime);
}

class Enemies {

public:

	Enemies();
	sf::RectangleShape enemyShape;
	sf::Vector2f enemyVelocity;
	void update(float deltaTime);

};

Enemies::Enemies() {
	enemyShape.setSize(sf::Vector2f(50 , 50));
	enemyShape.setFillColor(sf::Color::Red);
	enemyShape.setPosition( static_cast<float>(std::rand()%950 + 850), 600 - enemyShape.getSize().y);
	enemyVelocity.x = -(std::rand() % 100 + 50);
	enemyVelocity = sf::Vector2f(enemyVelocity.x, 0);
}

void Enemies::update(float deltaTime) {
	enemyShape.move(enemyVelocity * deltaTime);
}

class Game {

private:

	sf::RenderWindow window;
	std::vector<Bullets> bullets;
	std::vector<Enemies> enemies;
	int enemyCount = 10;
	bool spawnEnemy = false;
	//sf::Event event;

public:

    Game();
	float gravity = 1200.f;
	bool onGround = true;
	sf::Clock deltaTimeClock;
	float deltaTime = 0.f;
	sf::RectangleShape player;
	sf::Vector2f playerVelocity = sf::Vector2f(500, 0);
	sf::Vector2f jumpVelocity;

	void update();
	void handleEvents();
    void render();
	void rungame();

};

Game::Game() :window(sf::VideoMode(800, 600), "Soldier Defence")
{
	window.setFramerateLimit(60);
	deltaTime = 0.f;
	player.setSize(sf::Vector2f(50, 50));
	player.setFillColor(sf::Color::Green);
	player.setPosition( static_cast<float>(window.getSize().x) / 2 - player.getSize().x / 2, static_cast<float>(window.getSize().y) - player.getSize().y);

}

void Game::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}


		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && onGround)
		{
			onGround = false;
			playerVelocity.y = -600.f;
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {

			Bullets bullet;
			std::cout << "Bullet created" << std::endl;
			bullet.bulletShape.setPosition(player.getPosition().x + player.getSize().x, player.getPosition().y + player.getSize().y / 2 - bullet.bulletShape.getRadius());
			bullet.bulletVelocity = sf::Vector2f(800.f, 0.f);
			bullets.emplace_back(bullet);
			std::cout << "Number of bullets: " << bullets.size() << std::endl;
		}

	}
}

void Game::update()
{
	for (auto& bullet : bullets) {
		bullet.update(deltaTime);
	}

	for (auto& enemy : enemies) {
		enemy.update(deltaTime);
	}

	for (auto it = bullets.begin(); it != bullets.end();) {
		if (it->bulletShape.getPosition().x > window.getSize().x) {
			it = bullets.erase(it);
		}
		else {
			++it;
		}
	}

	for (int i = 0; i < bullets.size(); i++) {
		for (int j = 0; j < enemies.size(); j++) {
			if (bullets[i].bulletShape.getGlobalBounds().intersects(enemies[j].enemyShape.getGlobalBounds()) && enemies[j].enemyShape.getPosition().x <= window.getSize().x)	 {
				std::cout << "Bullet hit enemy" << std::endl;
				bullets.erase(bullets.begin() + i);
				enemies.erase(enemies.begin() + j);
				break;
			}
		}
	}

	if (enemies.size() == 0) {

		spawnEnemy = true;

		for ( int i = 0; i < enemyCount; i++) {
			Enemies enemy;
			std::cout << "Enemy created" << std::endl;
			enemies.emplace_back(enemy);
			std::cout << "Number of enemies: " << enemies.size() << std::endl;
		}

		spawnEnemy = false;
	}

	else 

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player.getPosition().x < 800 - player.getSize().x) {
		player.move(playerVelocity.x * deltaTime , 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player.getPosition().x > 0) {
		player.move(-playerVelocity.x * deltaTime , 0);
	}

	

	if (!onGround)
	{
		playerVelocity.y += gravity * deltaTime;
		player.move(0.f, playerVelocity.y * deltaTime);

		// Check if player reached the ground
		if (player.getPosition().y + player.getSize().y >= window.getSize().y)
		{
			player.setPosition(player.getPosition().x, window.getSize().y - player.getSize().y);
			playerVelocity.y = 0.f;
			onGround = true;
		}
	}
}

void Game::render()
{
	
		window.clear(sf::Color::Black);
		window.draw(player);
		
		for (auto& bullet : bullets) {
			window.draw(bullet.bulletShape);
		}

		for (auto& enemy : enemies) {
			window.draw(enemy.enemyShape);
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

int main()
{
	Game game;
	game.rungame();

	return 0;
}
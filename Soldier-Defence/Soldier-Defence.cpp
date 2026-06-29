#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>

// <--------------------------player-bullets----------------->
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

//<---------------------------enemy-bullets------------->

class EnemyBullets {

public:
	float enemyBulletRadius = 5.0f;
	sf::CircleShape enemyBulletShape;
	sf::Vector2f enemyBulletVelocity;

	EnemyBullets(sf::Vector2f position, sf::Vector2f direction);
	void update(float deltaTime);

};

EnemyBullets::EnemyBullets(sf::Vector2f position, sf::Vector2f direction)
{
	enemyBulletShape.setFillColor(sf::Color::Red);
	enemyBulletShape.setRadius(enemyBulletRadius);
	enemyBulletShape.setPosition(position);

	enemyBulletVelocity = direction * 500.f;
}


void EnemyBullets::update( float deltaTime){

	enemyBulletShape.move(enemyBulletVelocity * deltaTime);

}

//<-------------------------attacking enemies----------->
class AttackingEnemies {

public:

	sf::RectangleShape attackingEnemyShape;
	sf::Vector2f attackingEnemyVelocity;

	float shootTimer = 0.f;
	float shootCooldown = 3.0f;

	AttackingEnemies();

	void update(float deltaTime, sf::Vector2f objectPosition,
		std::vector<EnemyBullets>& enemyBullets , float defendObjectWidth);
};

AttackingEnemies::AttackingEnemies(){

	attackingEnemyShape.setPosition(static_cast<float>(std::rand() % 900) + 850.0, 400.0);
	attackingEnemyShape.setSize(sf::Vector2f(50 , 50));
	attackingEnemyVelocity.x = -(std::rand() % 80 + 50);
	attackingEnemyShape.setFillColor(sf::Color::Blue);

}

void AttackingEnemies::update(float deltaTime,sf::Vector2f objectPosition,std::vector<EnemyBullets>& enemyBullets , float defendObjectWidth)
{
	attackingEnemyShape.move(attackingEnemyVelocity * deltaTime);

	shootTimer += deltaTime;

	if (shootTimer >= shootCooldown)
	{

		if (attackingEnemyShape.getPosition().x > 0 && attackingEnemyShape.getPosition().x <= 600 && attackingEnemyShape.getPosition().x > objectPosition.x + defendObjectWidth) {

			sf::Vector2f target = objectPosition;  //enemy inaccuracy
			target.x += (std::rand() % 151) - 20;
			target.y += (std::rand() % 81) - 50;

			sf::Vector2f direction =
				target - attackingEnemyShape.getPosition();

			float length = sqrt(
				direction.x * direction.x +
				direction.y * direction.y
			);

			direction /= length;

			enemyBullets.emplace_back(
				attackingEnemyShape.getPosition(),
				direction);

			std::cout << "Enemy bullet created" << std::endl;
			std::cout << "Number of enemy bullets: " << enemyBullets.size() << std::endl;
			shootTimer = 0.f;
		}
	}
}
//<-------------------------------enemy------------------------------>
class Enemies {

public:

	Enemies();
	sf::RectangleShape enemyShape;
	sf::Vector2f enemyVelocity;
	void update(float deltaTime, std::vector<EnemyBullets> &enemyBullets, sf::Vector2f playerPosition);

	float shootTimer = 0.f;
	float shootCooldown = 1.5f;

};

Enemies::Enemies() {
	enemyShape.setSize(sf::Vector2f(50 , 50));
	enemyShape.setFillColor(sf::Color::Red);
	enemyShape.setPosition( static_cast<float>(std::rand()%950 + 850), 600 - enemyShape.getSize().y);
	enemyVelocity.x = -(std::rand() % 100 + 50);
	enemyVelocity = sf::Vector2f(enemyVelocity.x, 0);
}

void Enemies::update(float deltaTime, std::vector<EnemyBullets> &enemyBullets ,sf::Vector2f playerPosition){

	enemyShape.move(enemyVelocity * deltaTime);
	
}

//<---------------------------Game---------------------->
class Game {

private:

	sf::RenderWindow window;
	std::vector<Bullets> bullets;
	std::vector<Enemies> enemies;
	std::vector<AttackingEnemies> attackingEnemies;
	std::vector<EnemyBullets> enemyBullets;

	int enemyCount = 10;
	int attackingEnemyCount = 5;
	bool spawnEnemy = false;
	bool spawnAttackingEnemy = false;

public:

    Game();

	float gravity = 1200.f;
	bool onGround = true;
	sf::Clock deltaTimeClock;
	float deltaTime = 0.f;
	float shakeTime = 0.15f;

	sf::RectangleShape player;
	sf::RectangleShape defendObject;
	float defendObjectWidth;
	sf::RectangleShape defendObjectHealthShape;
	sf::Vector2f defendObjectPosition;
	float defendObjectHealthWidth;
	float defendObjectHealthHeight;
	float defendObjectHealth = 100.0f;
	sf::Vector2f playerVelocity = sf::Vector2f(500, 0);
	sf::Vector2f jumpVelocity;

	float windowHeight;

	void update();
	void handleEvents();
    void render();
	void rungame();

};

Game::Game() :window(sf::VideoMode(800, 600), "Soldier Defence")
{
	window.setFramerateLimit(60);
	deltaTime = 0.f;

	windowHeight = window.getSize().y;

	defendObject.setSize(sf::Vector2f(100, 150));
	defendObjectWidth = defendObject.getSize().x;
	defendObject.setFillColor(sf::Color::Blue);
	defendObject.setPosition(0, windowHeight - defendObject.getSize().y);
	defendObjectPosition = defendObject.getPosition();

	defendObjectHealthShape.setSize(sf::Vector2f(100, 10));
	defendObjectHealthWidth = defendObjectHealthShape.getSize().x;
	defendObjectHealthHeight = defendObjectHealthShape.getSize().y;
	defendObjectHealthShape.setFillColor(sf::Color::Red);
	defendObjectHealthShape.setPosition(0, windowHeight - defendObject.getSize().y - 30.0);

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
		enemy.update(deltaTime, enemyBullets, player.getPosition());
	}

	for (auto& attackingenemy : attackingEnemies)
	{
		attackingenemy.update(deltaTime, defendObject.getPosition(), enemyBullets , defendObjectWidth);
	}

	for (auto& bullets : enemyBullets) {
		bullets.update(deltaTime);
	}

	for (auto it = bullets.begin(); it != bullets.end();) {
		if (it->bulletShape.getPosition().x > window.getSize().x) {
			it = bullets.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
		if (it->enemyBulletShape.getPosition().y > window.getSize().y) {
			it = enemyBullets.erase(it);
		}
		else ++it;
	}

	for (int i = 0; i < bullets.size(); i++) {
		for (int j = 0; j < enemies.size(); j++) {
			if (bullets[i].bulletShape.getGlobalBounds().intersects(enemies[j].enemyShape.getGlobalBounds()) && enemies[j].enemyShape.getPosition().x <= window.getSize().x)	 {
				std::cout << "Bullet hit enemy" << std::endl;
				bullets.erase(bullets.begin() + i);
				i--;
				enemies.erase(enemies.begin() + j);
				j--;
				break;
			}
		}
	}

	for (int i = 0; i < bullets.size(); i++) {
		for (int j = 0; j < attackingEnemies.size(); j++) {
			if (bullets[i].bulletShape.getGlobalBounds().intersects(attackingEnemies[j].attackingEnemyShape.getGlobalBounds()) && attackingEnemies[j].attackingEnemyShape.getPosition().x < 800) {
				std::cout << "Bullet hit attacking enemy" << std::endl;
				bullets.erase(bullets.begin() + i);
				i--;
				attackingEnemies.erase(attackingEnemies.begin() + j);
				j--;
				break;
			}
		}
	}

	for (int i = 0; i < enemyBullets.size(); i++) {

		if (enemyBullets[i].enemyBulletShape.getGlobalBounds().intersects(defendObject.getGlobalBounds())) {
			defendObjectHealth-=5;
			defendObjectHealthShape.setSize(sf::Vector2f(defendObjectHealth , defendObjectHealthHeight));
			enemyBullets.erase(enemyBullets.begin() + i);
			i--;
			std::cout << "Enemy bullet destroyed" << std::endl;
			shakeTime = 0.15f;

			if (shakeTime > 0) {
				shakeTime -= deltaTime;
				float offset = (std::rand() % 7) - 3;
				defendObject.setPosition(defendObjectPosition.x + offset, defendObjectPosition.y);
			}
			else defendObject.setPosition(defendObjectPosition);
		}
	}

	for (int i = 0; i < attackingEnemies.size(); i++) {
		if (attackingEnemies[i].attackingEnemyShape.getPosition().x < 0) {
			attackingEnemies.erase(attackingEnemies.begin() + i);
			i--;
			std::cout << "Attacking enemy erased" << std::endl;
		}
	}

	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i].enemyShape.getPosition().x < 0) {
			enemies.erase(enemies.begin() + i);
			i--;
			std::cout << "Enemy destroyed" << std::endl;
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

	if (attackingEnemies.size() == 0) {

		spawnAttackingEnemy = true;

		for (int i = 0; i < attackingEnemyCount; i++) {

			AttackingEnemies attackingEnemyObj;
			std::cout << "Attacking enemy created" << std::endl;
			attackingEnemies.emplace_back(attackingEnemyObj);
			std::cout << "Number of attacking enemies: " << attackingEnemies.size() << std::endl;

		}

		spawnAttackingEnemy = false;

	}

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
		window.draw(defendObject);
		window.draw(defendObjectHealthShape);
		
		for (auto& bullet : bullets) {
			window.draw(bullet.bulletShape);
		}

		for (auto& enemy : enemies) {
			window.draw(enemy.enemyShape);
		}

		for (auto& enemy : attackingEnemies) {
			window.draw(enemy.attackingEnemyShape);
		}

		for (auto& bullets : enemyBullets){
			window.draw(bullets.enemyBulletShape);
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
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "Animations.h"
#include "Text.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

enum class GameState {
	About,
	Menu,
	Start,
	Pause,
	Gameover,
	Exit
};

// <--------------------------player-bullets----------------->
class Bullets {

public:

	Bullets();
	sf::CircleShape bulletShape;
	sf::Vector2f bulletVelocity;

	void update(float deltaTime , sf::Sprite &bulletSprite);

};

Bullets::Bullets() {

	float bulletRadius = 5.f;
	bulletShape.setRadius(bulletRadius);
	bulletVelocity = sf::Vector2f(800.f, 0.f);

}

void Bullets::update( float deltaTime , sf::Sprite &bulletSprite) {
	bulletShape.move(bulletVelocity * deltaTime);
	bulletSprite.setPosition(bulletShape.getPosition().x , bulletShape.getPosition().y-12);
}

//<---------------------------enemy-bullets------------->

class EnemyBullets {

public:
	float enemyBulletRadius = 5.0f;
	sf::CircleShape enemyBulletShape;
	sf::Vector2f enemyBulletVelocity;

	EnemyBullets(sf::Vector2f position, sf::Vector2f direction);
	void update(float deltaTime , sf::Sprite &enemyBulletSprite);

};

EnemyBullets::EnemyBullets(sf::Vector2f position, sf::Vector2f direction)
{
	enemyBulletShape.setFillColor(sf::Color::Red);
	enemyBulletShape.setRadius(enemyBulletRadius);
	enemyBulletShape.setPosition(position);

	enemyBulletVelocity = direction * 500.f;
}


void EnemyBullets::update( float deltaTime , sf::Sprite &enemyBulletSprite){

	enemyBulletShape.move(enemyBulletVelocity * deltaTime);
	enemyBulletSprite.setPosition(enemyBulletShape.getPosition());
}

//<-------------------------attacking enemies----------->
class AttackingEnemies {

public:

	sf::RectangleShape attackingEnemyShape;
	sf::Vector2f attackingEnemyVelocity;

	float shootTimer = 0.f;
	float shootCooldown = 3.0f;

	AttackingEnemies();
	Animation attackingEnemyMove;

	void update(float deltaTime, sf::Vector2f objectPosition,
		std::vector<EnemyBullets>& enemyBullets , float defendObjectWidth);
	void render(sf::RenderWindow& window);
};

AttackingEnemies::AttackingEnemies(){

	attackingEnemyShape.setPosition(static_cast<float>(std::rand() % 900) + 850.0, 400.0);
	attackingEnemyShape.setSize(sf::Vector2f(50 , 50));
	attackingEnemyVelocity.x = -(std::rand() % 80 + 50);
	attackingEnemyShape.setFillColor(sf::Color::Blue);

	attackingEnemyMove.load("resources/D2.png", 32, 32, 4, 0.15);
	attackingEnemyMove.getSprite().setScale(-3.f, 3.f);
	

}

void AttackingEnemies::update(float deltaTime,sf::Vector2f objectPosition,std::vector<EnemyBullets>& enemyBullets , float defendObjectWidth)
{
	attackingEnemyShape.move(attackingEnemyVelocity * deltaTime);
	attackingEnemyMove.update(deltaTime);
	attackingEnemyMove.getSprite().setPosition(attackingEnemyShape.getPosition());

	shootTimer += deltaTime;

	if (shootTimer >= shootCooldown)
	{

		if (attackingEnemyShape.getPosition().x > 0 && attackingEnemyShape.getPosition().x < 700 && attackingEnemyShape.getPosition().x > objectPosition.x + defendObjectWidth) {

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

			enemyBullets.emplace_back(attackingEnemyShape.getPosition() , direction);
			shootTimer = 0.f;
		}
	}
}

void AttackingEnemies::render(sf::RenderWindow& window) {
	window.draw(attackingEnemyMove.getSprite());
}

//<-------------------------------enemy------------------------------>
class Enemies {

public:

	Enemies();
	sf::RectangleShape enemyShape;
	sf::Vector2f enemyVelocity;
	void update(float deltaTime, std::vector<EnemyBullets> &enemyBullets, sf::Vector2f playerPosition);
	void render(sf::RenderWindow& window);

	float shootTimer = 0.f;
	float shootCooldown = 1.5f;
	bool isAttacking = false;
	Animation enemyIdleAnimation;

};

Enemies::Enemies()
{
	bool loaded = enemyIdleAnimation.load("resources/SMS_Soldier_RUN_WEST_strip4.png",16, 24, 4, 0.15);
	isAttacking = false;

	if (!loaded)
	{
		std::cout << "Animation failed to load\n";
	}

	enemyIdleAnimation.getSprite().setScale(5.f, 5.f);

	enemyShape.setSize(sf::Vector2f(50, 120));
	enemyShape.setFillColor(sf::Color::Red);

	enemyShape.setPosition(
		static_cast<float>(std::rand() % 950 + 850),
		600 - enemyShape.getSize().y
	);

	enemyVelocity.x = -(std::rand() % 100 + 50);
	enemyVelocity.y = 0;
}

void Enemies::update(float deltaTime, std::vector<EnemyBullets> &enemyBullets ,sf::Vector2f playerPosition){

	if (isAttacking) enemyIdleAnimation.update(deltaTime, true);
	else enemyIdleAnimation.update(deltaTime, false);
	enemyShape.move(enemyVelocity * deltaTime);
	enemyIdleAnimation.getSprite().setPosition(enemyShape.getPosition().x , enemyShape.getPosition().y);
	
}

void Enemies::render(sf::RenderWindow& window) {
	window.draw(enemyIdleAnimation.getSprite());
}

//<---------------------------Game---------------------->
class Game {

private:

	GameState state;
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
	int defendObjectHealth = 100;
	sf::RectangleShape playerHealthShape;
	float playerHealthWidth;
	float playerHealthHeight;
	int playerHealth = 100;
	sf::Vector2f playerVelocity = sf::Vector2f(500, 0);
	sf::Vector2f jumpVelocity;

	float windowHeight;
	float windowWidth;

	sf::Texture PlayeridleTexture;
	sf::Sprite playerSprite;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	sf::Texture weaponTexture;
	sf::Sprite weaponSprite;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	sf::Texture enemyBulletTexture;
	sf::Sprite enemyBulletSprite;
	sf::Texture defendObjectTexture;
	sf::Sprite defendObjectSprite;
	sf::Texture menuTexture;
	sf::Sprite menuSprite;
	sf::Texture bloodTexture;
	sf::Sprite bloodSprite;
	sf::Texture aboutTexture;
	sf::Sprite aboutSprite;
	bool showBlood = false;

	sf::Music backgroundMusic;
	sf::SoundBuffer shootSoundBuffer;
	sf::Sound shootSound;
	sf::SoundBuffer hitSoundBuffer;
	sf::Sound hitSound;
	sf::SoundBuffer gameoverSoundBuffer;
	sf::Sound gameoverSound;

	Animation idleAnimation;
	Text enemiesKilledText;
	Text enemyDronesKilledText;
	Text menuText;
	Text pauseText;
	Text objectiveText;
	Text gameoverText;
	Text playerHealthText;
	Text defendObjectHealthText;
	Text aboutText;
	Text startText;
	int enemiesKilled = 0;
	int enemyDronesKilled = 0;

	float animationTimer = 0.f;
	int currentFrame = 0;
	float animationSpeed = 0.18f;

	void update();
	void handleEvents();
    void render();
	void resetGame();
	void rungame();

};

Game::Game() :window(sf::VideoMode(800, 600), "Operation Iron Wall", sf::Style::Titlebar | sf::Style::Close)
{
	window.setFramerateLimit(60);
	deltaTime = 0.f;

	state = GameState::About;

	windowHeight = window.getSize().y;
	windowWidth = window.getSize().x;

	defendObject.setSize(sf::Vector2f(100, 150));
	defendObjectWidth = defendObject.getSize().x;
	defendObject.setFillColor(sf::Color::Blue);
	defendObject.setPosition(0, windowHeight - defendObject.getSize().y);
	defendObjectPosition = defendObject.getPosition();

	defendObjectHealthShape.setSize(sf::Vector2f(100, 10));
	defendObjectHealthWidth = defendObjectHealthShape.getSize().x;
	defendObjectHealthHeight = defendObjectHealthShape.getSize().y;
	defendObjectHealthShape.setFillColor(sf::Color::Red);
	defendObjectHealthShape.setPosition(10, windowHeight - defendObject.getSize().y+30);

	playerHealthShape.setSize(sf::Vector2f(playerHealth, 10));
	playerHealthHeight = playerHealthShape.getSize().y;
	playerHealthShape.setFillColor(sf::Color::Red);

	player.setSize(sf::Vector2f(50, 50));
	player.setFillColor(sf::Color::Green);
	player.setPosition(static_cast<float>(window.getSize().x) / 2 - player.getSize().x / 2, static_cast<float>(window.getSize().y) - player.getSize().y-5);

	backgroundTexture.loadFromFile("resources/War4.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(windowWidth / backgroundSprite.getGlobalBounds().width, windowHeight / backgroundSprite.getGlobalBounds().height);

	idleAnimation.load(
		"resources/NES_Soldier_IDLE_EAST_strip4.png",
		16,
		24,
		4,
		0.15f
	);
	idleAnimation.getSprite().setScale(5.f, 5.f);

	if (!weaponTexture.loadFromFile("resources/StG 44.png")) std::cout << "Failed to load weapon texture" << std::endl;
	weaponSprite.setTexture(weaponTexture);
	weaponSprite.setScale(2., 2.);

	bulletTexture.loadFromFile("resources/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setScale(0.015, 0.015);

	enemyBulletTexture.loadFromFile("resources/enemybullet.png");
	enemyBulletSprite.setTexture(enemyBulletTexture);
	enemyBulletSprite.setScale(-2., 2.);

	defendObjectTexture.loadFromFile("resources/tank.png");
	defendObjectSprite.setTexture(defendObjectTexture);
	defendObjectSprite.setScale(4., 4.);

	menuTexture.loadFromFile("resources/War.png");
	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(windowWidth / menuSprite.getGlobalBounds().width, windowHeight / menuSprite.getGlobalBounds().height);

	bloodTexture.loadFromFile("resources/Effect_4.png");
	bloodSprite.setTexture(bloodTexture);
	bloodSprite.setScale(windowWidth / bloodSprite.getGlobalBounds().width, windowHeight / bloodSprite.getGlobalBounds().height);

	aboutTexture.loadFromFile("resources/WCP_Example.png");
	aboutSprite.setTexture(aboutTexture);
	aboutSprite.setScale(windowWidth / aboutSprite.getGlobalBounds().width, windowHeight / aboutSprite.getGlobalBounds().height);

	enemiesKilledText.addDetails("Enemies Killed: 0","resources/PixelOperatorMonoHB8.ttf", 16, sf::Color::Yellow, sf::Vector2f(10, 80));
	enemyDronesKilledText.addDetails("Drones Killed: 0", "resources/PixelOperatorMonoHB8.ttf", 16, sf::Color::Yellow, sf::Vector2f(10, 50));

	menuText.addDetails("START (Enter)", "resources/PixelOperatorMonoHB8.ttf", 30, sf::Color(0,0,82), sf::Vector2f(windowWidth / 2 - 150.f, windowHeight / 2));
	pauseText.addDetails("PAUSED (P to resume)", "resources/PixelOperatorMonoHB8.ttf", 30, sf::Color(0, 0, 82), sf::Vector2f(windowWidth / 2 - 280.f, windowHeight / 2));
	objectiveText.addDetails("Defend the Tank!", "resources/PixelOperatorMonoHB8.ttf", 16, sf::Color::Cyan, sf::Vector2f(windowWidth / 2 - 100.f, 10));
	gameoverText.addDetails("GAME OVER!", "resources/PixelOperatorMonoHB8.ttf", 24, sf::Color::Red, sf::Vector2f(windowWidth / 2 - 120.f, windowHeight / 2));
	playerHealthText.addDetails("Player Health: 100", "resources/PixelOperatorMonoHB8.ttf", 16, sf::Color::Red, sf::Vector2f(windowWidth - 300.f, 50));
	defendObjectHealthText.addDetails("Tank Health: 100", "resources/PixelOperatorMonoHB8.ttf", 16, sf::Color::Red, sf::Vector2f(windowWidth - 300.f, 80));
	aboutText.addDetails("Som Kashyap presents: \n Operation Iron Wall", "resources/PixelOperatorMonoHB8.ttf", 24, sf::Color::White, sf::Vector2f(windowWidth / 2 - 250.f, windowHeight / 2));
	startText.addDetails("START: Enter", "resources/PixelOperatorMonoHB8.ttf", 16, sf::Color::Blue, sf::Vector2f(windowWidth / 2 - 250.f, windowHeight / 2+ 100.));

	backgroundMusic.openFromFile("resources/williamhector-horde-war-drums-loop-130bpm-342956.ogg");
	backgroundMusic.setLoop(true);
	backgroundMusic.setVolume(100.f);
	backgroundMusic.play();

	shootSoundBuffer.loadFromFile("resources/tuomas_data-gun-shot-1-176892.ogg");
	shootSound.setBuffer(shootSoundBuffer);
	shootSound.setVolume(50.f);

	hitSoundBuffer.loadFromFile("resources/voicebosch-missile-explosion-168600.ogg");
	hitSound.setBuffer(hitSoundBuffer);
	hitSound.setVolume(100.f);

	gameoverSoundBuffer.loadFromFile("resources/universfield-game-over-deep-male-voice-clip-352695.ogg");
	gameoverSound.setBuffer(gameoverSoundBuffer);
	gameoverSound.setVolume(100.f);
}

void Game::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (state == GameState::Start) {

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && onGround)
			{
				onGround = false;
				playerVelocity.y = -600.f;
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {

				Bullets bullet;
				bullet.bulletShape.setPosition(player.getPosition().x + player.getSize().x, player.getPosition().y + player.getSize().y / 2 - bullet.bulletShape.getRadius() - 15);
				bullet.bulletVelocity = sf::Vector2f(800.f, 0.f);
				bullets.emplace_back(bullet);
				shootSound.play();
			}

		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Enter)
			{
				if (state == GameState::Menu)
				{
					state = GameState::Start;
					resetGame();
					backgroundMusic.play();
				}
				else if (state == GameState::About)
				{
					state = GameState::Menu;
				}
				else if (state == GameState::Gameover)
				{
					state = GameState::Menu;
					resetGame();
					backgroundMusic.play();
				}
			}

			if (event.key.code == sf::Keyboard::P) {
				if (state == GameState::Start) {
					state = GameState::Pause;
					backgroundMusic.pause();
				}
				else if (state == GameState::Pause) {
					state = GameState::Start;
					backgroundMusic.play();
				}
			}
			if (event.key.code == sf::Keyboard::Escape) window.close();
		}
	}
}

void Game::update()
{
	playerHealthShape.setPosition(player.getPosition().x+15, player.getPosition().y - 80.0);
	defendObjectSprite.setPosition(defendObject.getPosition().x ,defendObject.getPosition().y+defendObjectSprite.getGlobalBounds().height-25.0);
	weaponSprite.setPosition(player.getPosition().x + 20, player.getPosition().y + player.getGlobalBounds().height / 2-30.0);
	idleAnimation.update(deltaTime);
	idleAnimation.getSprite().setPosition(player.getPosition().x, player.getPosition().y - 55.f);

	if (state == GameState::Start) {

		if (defendObjectHealth <= 50) {
			showBlood = true;
		}

		for (auto& bullet : bullets) {
			bullet.update(deltaTime, bulletSprite);
		}

		for (auto& enemy : enemies) {
			enemy.update(deltaTime, enemyBullets, player.getPosition());
		}

		for (auto& attackingenemy : attackingEnemies)
		{
			attackingenemy.update(deltaTime, defendObjectSprite.getPosition(), enemyBullets, defendObjectWidth);
		}

		for (auto& bullets : enemyBullets) {
			bullets.update(deltaTime, enemyBulletSprite);
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

		for (auto bullet = bullets.begin(); bullet != bullets.end(); )
		{
			bool hit = false;

			for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
			{
				if (bullet->bulletShape.getGlobalBounds()
					.intersects(enemy->enemyShape.getGlobalBounds()))
				{
					enemies.erase(enemy);
					bullet = bullets.erase(bullet);
					hit = true;
					enemiesKilled++;
					enemiesKilledText.toString("Enemies Killed: " + std::to_string(enemiesKilled));
					break;

				}
			}

			if (!hit)
				bullet++;
		}
	}
		

		for (int i = 0; i < bullets.size(); i++) {
			for (int j = 0; j < attackingEnemies.size(); j++) {
				if (bullets[i].bulletShape.getGlobalBounds().intersects(attackingEnemies[j].attackingEnemyShape.getGlobalBounds()) && attackingEnemies[j].attackingEnemyShape.getPosition().x < 800) {
					enemyDronesKilled++;
					enemyDronesKilledText.toString("Drones Killed: " + std::to_string(enemyDronesKilled));
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

				if (defendObjectHealth > 0) defendObjectHealth -= 5;

				defendObjectHealthText.toString("Tank Health: " + std::to_string(defendObjectHealth));

				enemyBullets.erase(enemyBullets.begin() + i);
				i--;

				shakeTime = 0.15f;
				hitSound.play();

				if (defendObjectHealth <= 0) {
					state = GameState::Gameover;
					gameoverSound.play();
					backgroundMusic.stop();
					return;   
				}

				defendObjectHealthShape.setSize(sf::Vector2f(defendObjectHealth, defendObjectHealthHeight));

			}
		}

		if (shakeTime > 0) {
			shakeTime -= deltaTime;
			float offset = (float)(std::rand() % 7) - 3;
			defendObject.setPosition(defendObjectPosition.x + offset, defendObjectPosition.y);
		}
		else {
			defendObject.setPosition(defendObjectPosition);
		}

		for (int i = 0; i < enemies.size(); i++) {
			if (enemies[i].enemyShape.getGlobalBounds().intersects(defendObject.getGlobalBounds())) {
				enemies[i].isAttacking = true;
				if(defendObjectHealth > 0)  defendObjectHealth -= 1;
				defendObjectHealthText.toString("Tank Health: " + std::to_string(defendObjectHealth));
				if (defendObjectHealth <= 0) {
					state = GameState::Gameover;
					resetGame();
					player.setPosition(static_cast<float>(window.getSize().x) / 2 - player.getSize().x / 2, static_cast<float>(window.getSize().y) - player.getSize().y - 20);
					gameoverSound.play();
					backgroundMusic.stop();
				}
				enemies[i].enemyVelocity = (sf::Vector2f(0, 0));
				defendObjectHealthShape.setSize(sf::Vector2f(defendObjectHealth, defendObjectHealthHeight));

			}
			else if (enemies[i].enemyShape.getGlobalBounds().intersects(player.getGlobalBounds())) {
				enemies[i].isAttacking = true;
				if (playerHealth > 0) playerHealth -= 1;
				playerHealthText.toString("Player Health: " + std::to_string(playerHealth));
				playerHealthShape.setSize(sf::Vector2f(playerHealth, playerHealthHeight));
				if (playerHealth <= 0 ) {
					state = GameState::Gameover;
					resetGame();
					gameoverSound.play();
					backgroundMusic.stop();
				}
				enemies[i].enemyVelocity = (sf::Vector2f(0, 0));
				
			}

			else {
				enemies[i].isAttacking = false;
				enemies[i].enemyVelocity.x = -100;
				enemies[i].enemyVelocity.y = 0.f;
			}
		}

		for (int i = 0; i < attackingEnemies.size(); i++) {
			if (attackingEnemies[i].attackingEnemyShape.getPosition().x < 0) {
				attackingEnemies.erase(attackingEnemies.begin() + i);
				i--;
			}
		}

		for (int i = 0; i < enemies.size(); i++) {
			if (enemies[i].enemyShape.getPosition().x < 0) {
				enemies.erase(enemies.begin() + i);
				i--;
			}
		}

		if (enemies.size() == 0) {

			spawnEnemy = true;
			enemies.reserve(enemyCount);

			for (int i = 0; i < enemyCount; i++) {
				//Enemies enemy;
				enemies.emplace_back();
			}

			spawnEnemy = false;
		}

		if (attackingEnemies.size() == 0) {

			spawnAttackingEnemy = true;

			for (int i = 0; i < attackingEnemyCount; i++) {

				AttackingEnemies attackingEnemyObj;
				attackingEnemies.emplace_back();
				

			}

			spawnAttackingEnemy = false;

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player.getPosition().x < 800 - player.getSize().x) {
			player.move(playerVelocity.x * deltaTime, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player.getPosition().x > 0) {
			player.move(-playerVelocity.x * deltaTime, 0);
		}



		if (!onGround)
		{
			playerVelocity.y += gravity * deltaTime;
			player.move(0.f, playerVelocity.y * deltaTime);

			// Check if player reached the ground
			if (player.getPosition().y + player.getSize().y >= window.getSize().y)
			{
				player.setPosition(player.getPosition().x, window.getSize().y - player.getSize().y - 15);
				playerVelocity.y = 0.f;
				onGround = true;
			}
		}

		animationTimer += deltaTime;

		if (animationTimer >= animationSpeed)
		{
			animationTimer = 0.f;

			currentFrame = (currentFrame + 1) % 4;

			playerSprite.setTextureRect(
				sf::IntRect(
					currentFrame * 16,
					0,
					16,
					24
				)
			);
		}
	}
	

void Game::resetGame() {

	playerHealth = 100;
	defendObjectHealth = 100;
	playerHealthText.toString("Player Health: 100");
	defendObjectHealthText.toString("Tank Health: 100");
	player.setPosition(static_cast<float>(window.getSize().x) / 2 - player.getSize().x / 2, static_cast<float>(window.getSize().y) - player.getSize().y - 20);
	enemiesKilled = 0;
	enemyDronesKilled = 0;
	enemiesKilledText.toString("Enemies Killed: 0");
	enemyDronesKilledText.toString("Enemy Drones Killed: 0");
	playerHealthShape.setSize(sf::Vector2f(playerHealth, playerHealthHeight));
	defendObjectHealthShape.setSize(sf::Vector2f(defendObjectHealth, defendObjectHealthHeight));
	enemies.clear();
	bullets.clear();
	attackingEnemies.clear();
	enemyBullets.clear();
	showBlood = false;

}

void Game::render()
{
	window.clear(sf::Color::Black);

	

	if (state == GameState::Start) {

		window.draw(backgroundSprite);
		if (showBlood) window.draw(bloodSprite);
		window.draw(idleAnimation.getSprite());
		window.draw(weaponSprite);
		window.draw(defendObjectSprite);
		window.draw(defendObjectHealthShape);
		window.draw(playerHealthShape);
		window.draw(enemiesKilledText.getText());
		window.draw(enemyDronesKilledText.getText());
		window.draw(objectiveText.getText());
		window.draw(playerHealthText.getText());
		window.draw(defendObjectHealthText.getText());

		for (auto& bullet : bullets) {
			window.draw(bulletSprite);
		}

		for (auto& enemy : enemies) {
			enemy.render(window);
		}

		for (auto& enemy : attackingEnemies) {
			enemy.render(window);
		}

		for (auto& bullets : enemyBullets) {
			window.draw(enemyBulletSprite);
		}
	}

	else if (state == GameState::Menu) {
		window.draw(menuSprite);
		window.draw(menuText.getText());
	}

	else if (state == GameState::Pause) {
		window.draw(menuSprite);
		window.draw(pauseText.getText());
		
	}

	else if (state == GameState::Gameover) {
			window.draw(bloodSprite);
		window.draw(gameoverText.getText());
	}
	else if (state == GameState::About) {
		window.draw(aboutSprite);
		window.draw(aboutText.getText());
		window.draw(startText.getText());
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
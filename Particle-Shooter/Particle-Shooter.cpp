#include<SFML/Graphics.hpp>
#include<ctime>
#include<cstdlib>
#include<iostream>

enum class GameState
{
	Menu,
	Start,
	Pause,
	GameOver,
	Exit
};

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
	void update(float deltaTime , bool respawn);
};

class Game {

public:
	GameState state;

	// Window dimensions
	const float windowWidth = 800.f, windowHeight = 600.f;
	sf::RenderWindow window;
	sf::RectangleShape cannon;

	// Cannon properties
	sf::Vector2f cannonVelocity;
	std::vector<Enemy>enemies;
	const int enemyCount = 10;
	std::vector<Particles>bullets;

	// Time management
	sf::Clock deltaTimeClock;
	float deltaTime = 0.f;
	bool respawn;
	sf::Clock timeClock;
	int time = 0;
	const float width = 20.f, height = 80.f;

	void UI();	
	void rungame();
	Game();

private:
	sf::Font font;
	sf::Text menuText;
	sf::Text pauseText;
	sf::Text exitText;
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

	GameState state = GameState::Menu;

	UI();

	for(size_t i = 0 ;i < enemyCount; i++) {
		Enemy enemy;
		enemies.emplace_back(enemy);
	}
	time = timeClock.getElapsedTime().asSeconds();
}

Enemy::Enemy() {

	//if (respawn) {
	//	enemyVelocity.y += 900 * deltaTime;
	//}
	 enemyVelocity = { 0 , static_cast<float>(rand()%50+10)};
	const float enemyWidth = 40.f, enemyHeight = 40.f;
	enemy.setSize(sf::Vector2f(enemyWidth, enemyHeight));
	enemy.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	enemy.setPosition(static_cast<float>(std::rand() %760) , static_cast<float>(std::rand()%-100));

}

void Particles::spawnParticles() {

	bulletVelocity = { 0,-500.0 };
	bullet.setFillColor(sf::Color::White);
	bullet.setRadius(5.0);

}

void Particles::Update(float deltaTime) {

	bullet.move(bulletVelocity * deltaTime);
}

void Enemy::update(float deltaTime , bool respawn) {

	enemy.move(enemyVelocity * deltaTime);

}

void Game::UI() {

	if (!font.loadFromFile("resources/arial.ttf")) {
		std::cout << "Error loading font" << std::endl;
	}
	
	menuText.setFont(font);
	menuText.setString("Press Enter to Start");
	menuText.setCharacterSize(24);
	menuText.setFillColor(sf::Color::White);
	menuText.setPosition(windowWidth / 2.f - menuText.getGlobalBounds().width / 2.f, windowHeight / 2.f - menuText.getGlobalBounds().height / 2.f);
	
	pauseText.setFont(font);
	pauseText.setString("Game Paused. Press P to Resume");
	pauseText.setCharacterSize(24);
	pauseText.setFillColor(sf::Color::White);
	pauseText.setPosition(windowWidth / 2.f - pauseText.getGlobalBounds().width / 2.f, windowHeight / 2.f - pauseText.getGlobalBounds().height / 2.f);

	exitText.setFont(font);
	exitText.setString("Press Escape to Exit");
	exitText.setCharacterSize(24);
	exitText.setFillColor(sf::Color::White);
	exitText.setPosition(windowWidth / 2.f - exitText.getGlobalBounds().width / 2.f, (windowHeight / 2.f - exitText.getGlobalBounds().height / 2.f) + 2*exitText.getGlobalBounds().height);
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
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {
			Particles newBullet;
			newBullet.spawnParticles();
			newBullet.bullet.setPosition(cannon.getPosition().x + (width / 2.0), cannon.getPosition().y);
			bullets.emplace_back(newBullet);
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {

			if (state == GameState::Menu) {
				enemies.clear();
				state = GameState::Start;
			}
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
			if (state == GameState::Start) {
				state = GameState::Pause;
			}
			else if (state == GameState::Pause) {
				state = GameState::Start;
			}
		}

		if (state == GameState::Start) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				state = GameState::Menu;
		
			}
		}
		else if (state == GameState::Menu) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				window.close();
			}
		}
		else if (state == GameState::Pause) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				state = GameState::Menu;
			}
		}

	}
}
void Game::update() {

	if (state == GameState::Start) {

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
			enemy.update(deltaTime, respawn);
		}

		for (size_t i = 0; i < enemies.size(); i++) {
			for (size_t j = 0; j < bullets.size(); j++) {
				if (enemies[i].enemy.getGlobalBounds().intersects(bullets[j].bullet.getGlobalBounds())) {
					enemies.erase(enemies.begin() + i);
					i--; // adjust index after erase
					bullets.erase(bullets.begin() + j);
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
			respawn = true;
			for (size_t i = 0; i < enemyCount; i++) {
				Enemy enemy;
				enemies.emplace_back(enemy);
			}

		}
		else {
			respawn = false;
		}
	}
	
	}

void Game::render() {

		window.clear();
	
		if (state == GameState::Start) {
			window.draw(cannon);
			for (auto& val : bullets) {
				window.draw(val.bullet);
			}

			for (auto& enemy : enemies) {
				window.draw(enemy.enemy);
			}
		}
		
		if (state == GameState::Menu) {
			window.draw(menuText);
			window.draw(exitText);
		}

		if (state == GameState::Pause) {
			window.draw(pauseText);
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
#include<SFML/Graphics.hpp>
#include<ctime>
#include<cstdlib>
#include<iostream>
#include<format>
#include<fstream>

enum class GameState
{
	About,
	Menu,
	Start,
	//BeginGame,
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
	sf::Sprite bulletsprite;
};

class Enemy {
public:
	Enemy();
	sf::Vector2f enemyVelocity;
	sf::RectangleShape enemy;
	//void createEnemy();
	void update(float deltaTime , bool respawn);

	sf::Sprite enemysprite;
};

class Game {
private:
	int highestScore;
	int score = 0;
	int wave = 0;
	int bulletsFired = 0;
	int bulletsHit = 0;
	float accuracy = 0.f;

	sf::Texture enemyTexture;
	sf::Texture cannonTexture;
	sf::Sprite cannonSprite;
	sf::Texture bulletTexture;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	sf::Texture menuTexture;
	sf::Sprite menuSprite;

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
	bool bonusDisplayed = false;
	int time = 0;
	const float width = 20.f, height = 80.f;
	
	void UI();	
	void rungame();
	void resetGame();
	Game();

private:
	sf::Font font;
	sf::Text menuText;
	sf::Text pauseText;
	sf::Text exitText;
	sf::Text scoreText;
	sf::Text waveText;
	sf::Text gameoverText;
	sf::Text bonusText;
	sf::Text bulletsFiredText;
	sf::Text bulletsHitText;
	sf::Text accuracyText;
	sf::Text highestScoreText;
	sf::Text aboutText;
	sf::Text beginGameText;
	sf::Text quoteText;

	void update();
	void handleEvents();
	void render();
	
};

Game::Game() :window(sf::VideoMode(windowWidth, windowHeight), "Space Warz") {

	window.setFramerateLimit(60);

	cannon.setSize(sf::Vector2f(20.f, 80.f));
	cannonVelocity = { 500.f , 0.f };
	//cannon.setFillColor(sf::Color::Green);
	cannon.setPosition(400.f, 520.f);

	state = GameState::About;

	UI();

	for (size_t i = 0; i < enemyCount; i++) {
		Enemy enemy;
		enemies.emplace_back(enemy);
	}
	time = timeClock.getElapsedTime().asSeconds();

	std::ifstream file("resources/highscore.txt");
	if (file.is_open()) {
		file >> highestScore;
		file.close();
	}

	if (!enemyTexture.loadFromFile("resources/enemyRed1.png")) std::cout << "Couldn't load enemyTexture!" << std::endl;

	if (!cannonTexture.loadFromFile("resources/playerShip1_blue.png")) std::cout << "Couldn't load cannonTexture!" << std::endl;
	if (!bulletTexture.loadFromFile("resources/laserBlue07.png")) std::cout << "Couldn't load bulletTexture!" << std::endl;
	if (!backgroundTexture.loadFromFile("resources/purple.png")) std::cout << "Couldn't load backgroundTexture!" << std::endl;
	if (!menuTexture.loadFromFile("resources/blue.png")) std::cout << "Couldn't load menuTexture!" << std::endl;


	cannonSprite.setTexture(cannonTexture);
	cannonSprite.setPosition(cannon.getPosition());

	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(window.getSize().x / backgroundSprite.getGlobalBounds().width, window.getSize().y / backgroundSprite.getGlobalBounds().height);

	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(window.getSize().x/2.f - menuSprite.getGlobalBounds().width/2.f, window.getSize().y/2.f - menuSprite.getGlobalBounds().height/2.f);

}

Enemy::Enemy() {

	//if (respawn) {
	//	enemyVelocity.y += 900 * deltaTime;
	//}
	enemyVelocity = { 0 , static_cast<float>(rand()%80+30)};
	const float enemyWidth = 40.f, enemyHeight = 40.f;
	enemy.setSize(sf::Vector2f(enemyWidth, enemyHeight));
	enemy.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	
	sf::Sprite enemysprite;

}

void Particles::spawnParticles() {

	bulletVelocity = { 0,-500.0 };
	bullet.setFillColor(sf::Color::White);
	bullet.setRadius(5.0);
	
}

void Particles::Update(float deltaTime) {

	bullet.move(bulletVelocity * deltaTime);
	bulletsprite.setPosition(bullet.getPosition());
}

void Enemy::update(float deltaTime , bool respawn) {

	enemy.move(enemyVelocity * deltaTime);
	enemysprite.setPosition(enemy.getPosition());

}

void Game::UI() {

	if (!font.loadFromFile("resources/arial.ttf")) std::cout << "Error loading font" << std::endl;
	
	menuText.setFont(font);
	menuText.setString("  '   SPACE WARZ   ' \n Hit Enter to Begin Hunting \n Press Esc to Exit ");
	menuText.setCharacterSize(20);
	menuText.setFillColor(sf::Color::White);
	menuText.setPosition(windowWidth / 2.f - menuText.getGlobalBounds().width / 2.f, windowHeight / 2.f - menuText.getGlobalBounds().height / 2.f);
	menuText.setLineSpacing(1.5f);
	menuText.setStyle(sf::Text::Bold);
	menuText.setOutlineThickness(0.5f);
	menuText.setOutlineColor(sf::Color::White);
	
	pauseText.setFont(font);
	pauseText.setString("Game Paused. Press P to Resume");
	pauseText.setCharacterSize(20);
	pauseText.setFillColor(sf::Color(255 , 215 ,0));
	pauseText.setPosition(windowWidth / 2.f - pauseText.getGlobalBounds().width / 2.f, windowHeight / 2.f - pauseText.getGlobalBounds().height / 2.f);
	pauseText.setStyle(sf::Text::Bold);
	pauseText.setOutlineThickness(2.0f);

	scoreText.setFont(font);
	scoreText.setString("Score: 0");
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::Yellow);
	scoreText.setPosition(10.f, 10.f);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setOutlineThickness(2.0f);

	waveText.setFont(font);
	waveText.setString("Wave: 1");
	waveText.setCharacterSize(20);
	waveText.setFillColor(sf::Color::Yellow);
	waveText.setPosition(10.f, 100.f);
	waveText.setStyle(sf::Text::Bold);
	waveText.setOutlineThickness(2.0f);

	gameoverText.setFont(font);
	gameoverText.setString("Game Over! Hit enter to return to menu");
	gameoverText.setCharacterSize(25);
	gameoverText.setFillColor(sf::Color::Red);
	gameoverText.setPosition(windowWidth / 2.f - gameoverText.getGlobalBounds().width / 2.f, (windowHeight / 2.f - gameoverText.getGlobalBounds().height / 2.f));
	gameoverText.setStyle(sf::Text::Bold);
	gameoverText.setOutlineThickness(0.5f);
	gameoverText.setOutlineColor(sf::Color::Red);

	//bonusText.setFont(font);
	//bonusText.setString("Boom +100 points!");
	//bonusText.setCharacterSize(20);
	//bonusText.setFillColor(sf::Color::Yellow);
	//bonusText.setPosition(windowWidth / 2.f - bonusText.getGlobalBounds().width / 2.f, (windowHeight / 2.f - bonusText.getGlobalBounds().height / 2.f));

	bulletsFiredText.setFont(font);
	bulletsFiredText.setString("Bullets Fired: 0");
	bulletsFiredText.setCharacterSize(20);
	bulletsFiredText.setFillColor(sf::Color::Yellow);
	bulletsFiredText.setPosition(10.f, 40.f);
	bulletsFiredText.setOutlineThickness(2.0f);

	bulletsHitText.setFont(font);
	bulletsHitText.setString("Bullets Hit: 0");
	bulletsHitText.setCharacterSize(20);
	bulletsHitText.setFillColor(sf::Color::Yellow);
	bulletsHitText.setPosition(10.f, 70.f);
	bulletsHitText.setOutlineThickness(2.0f);

	accuracyText.setFont(font);
	accuracyText.setString("Accuracy: 0%");
	accuracyText.setCharacterSize(20);
	accuracyText.setFillColor(sf::Color::Yellow);
	accuracyText.setPosition(10.f, 125.f);
	accuracyText.setOutlineThickness(2.0f);

	highestScoreText.setFont(font);
	//highestScoreText.setString("Highest Score: " + std::to_string(highestScore));
	highestScoreText.setCharacterSize(20);
	highestScoreText.setFillColor(sf::Color::Yellow);
	highestScoreText.setPosition(10.f, 150.f);
	highestScoreText.setOutlineThickness(2.0f);

	aboutText.setFont(font);
	aboutText.setString("Som Kashyap presents: \n ' SPACE WARZ ' \n [Hit Enter to Start] ");
	aboutText.setCharacterSize(20);
	aboutText.setFillColor(sf::Color::White);
	aboutText.setPosition(windowWidth / 2.f - aboutText.getGlobalBounds().width / 2.f, (windowHeight / 2.f - aboutText.getGlobalBounds().height / 2.f));
	aboutText.setLineSpacing(1.5f);
	aboutText.setStyle(sf::Text::Bold);
	aboutText.setLetterSpacing(1.5f);
	aboutText.setOutlineThickness(2.0f);

	quoteText.setFont(font);
	quoteText.setString(" Accuracy wins battles. Panic firing loses them. ");
	quoteText.setCharacterSize(20);
	quoteText.setFillColor(sf::Color::White);
	quoteText.setPosition(windowWidth / 2.f - quoteText.getGlobalBounds().width / 2.f, (windowHeight / 2.f - quoteText.getGlobalBounds().height / 2.f)+200.f);
	quoteText.setLineSpacing(1.5f);
	quoteText.setStyle(sf::Text::Italic);
	quoteText.setOutlineThickness(2.0f);

	//beginGameText.setFont(font);
	//beginGameText.setString("Hit Enter to Begin!");
	//beginGameText.setCharacterSize(20);
	//beginGameText.setFillColor(sf::Color::Yellow);
	//beginGameText.setPosition(windowWidth / 2.f - beginGameText.getGlobalBounds().width / 2.f, (windowHeight / 2.f - beginGameText.getGlobalBounds().height / 2.f));
	//beginGameText.setStyle(sf::Text::Bold);
	//beginGameText.setOutlineThickness(2.0f);
}

void Game::handleEvents() {

	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (state == GameState::Start) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && cannonSprite.getPosition().x > 0) {
				cannonSprite.move(cannonVelocity * (-deltaTime));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && cannonSprite.getPosition().x+cannonSprite.getGlobalBounds().width / 2.f < windowWidth) {
				cannonSprite.move(cannonVelocity * (deltaTime));
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {
				bulletsFired++;
				bulletsFiredText.setString("Bullets Fired: " + std::to_string(bulletsFired));
				Particles newBullet;
				newBullet.spawnParticles();

				float x = cannonSprite.getPosition().x +
					cannonSprite.getGlobalBounds().width / 2.f;
				float y = cannonSprite.getPosition().y;

				newBullet.bullet.setPosition(x, y);
				newBullet.bulletsprite.setPosition(x, y);

				newBullet.bulletsprite.setTexture(bulletTexture);

				bullets.emplace_back(newBullet);
			}
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
				resetGame();
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
		else if (state == GameState::GameOver) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
				state = GameState::Menu;
			}
		}
		else if (state == GameState::About) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
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

			if (enemies[i].enemy.getPosition().y +
				enemies[i].enemy.getSize().y > windowHeight)
			{

				state = GameState::GameOver;
				resetGame();
				return;

			}

			for (size_t j = 0; j < bullets.size(); j++) {

				if (enemies[i].enemysprite.getGlobalBounds().intersects(
					bullets[j].bullet.getGlobalBounds()))
				{
					score += 10;
					
					if (score > highestScore) {
						highestScore = score;
						std::ofstream file("resources/highscore.txt");
						file << highestScore;
					}

					highestScoreText.setString("Highest Score: " + std::to_string(highestScore));
					bulletsHit++;

					scoreText.setString("Score: " + std::to_string(score));
					bulletsHitText.setString("Bullets Hit: " + std::to_string(bulletsHit));

					accuracy = (static_cast<float>(bulletsHit) / static_cast<float>(bulletsFired)) * 100;
					std::string formattedAccuracy = std::format("{:.2f}", accuracy);
					accuracyText.setString("Accuracy: " + formattedAccuracy + "%");

					enemies.erase(enemies.begin() + i);
					i--;

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
			wave++;
			waveText.setString("Wave: " + std::to_string(wave));

			for (int i = 0; i < enemyCount; i++) {
				Enemy Enemy;
				Enemy.enemysprite.setTexture(enemyTexture);
				Enemy.enemy.setPosition(static_cast<float>(std::rand() % 760), -(i*50));
				enemies.emplace_back(Enemy);
			}

		}
		else respawn = false;

	}
	if (state == GameState::Start && enemies.size() == 0) {
		respawn = true;
		bonusDisplayed = true;
		bonusText.setString("Bonus +100 points!");

		if (timeClock.getElapsedTime().asSeconds() >= 2 && bonusDisplayed) {
			score += 100;
			scoreText.setString("Score: " + std::to_string(score));
			bonusDisplayed = false;
			timeClock.restart();
		}
	}
}

void Game::resetGame() {
	score = 0;
	wave = 0;
	bulletsFired = 0;
	bulletsHit = 0;
	accuracy = 0.f;
	accuracyText.setString("Accuracy: 0%");
	bulletsHitText.setString("Bullets Hit: " + std::to_string(bulletsHit));
	bulletsFiredText.setString("Bullets Fired: " + std::to_string(bulletsFired));
	scoreText.setString("Score: " + std::to_string(score));
	waveText.setString("Wave: " + std::to_string(wave));
	enemies.clear();
	bullets.clear();
}

void Game::render() {

		window.clear();
		if (state == GameState::Menu) {
			window.draw(menuSprite);
			window.draw(menuText);
			window.draw(quoteText);
		}
		if (state == GameState::About) {
			window.draw(aboutText);
		}
		if (state != GameState::Menu && state != GameState::About) {
			window.draw(backgroundSprite);
		}
		if (state == GameState::Pause) {
			window.draw(pauseText);
		}
		if (state == GameState::GameOver) {
			window.draw(gameoverText);
		}
		
		if (state == GameState::Start) {
			for (auto& enemy : enemies) {
				window.draw(enemy.enemysprite);
			}
			window.draw(cannonSprite);
			window.draw(scoreText);
			window.draw(waveText);
			window.draw(bulletsFiredText);
			window.draw(bulletsHitText);
			window.draw(accuracyText);
			window.draw(highestScoreText);

			if (bonusDisplayed) {
				window.draw(bonusText);
			}
			for (auto& val : bullets) {
				window.draw(val.bulletsprite);
			}


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
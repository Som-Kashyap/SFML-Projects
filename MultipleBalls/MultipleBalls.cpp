#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Ball {

    sf::CircleShape shape;
    sf::Vector2f velocity;

};

int main() {
   
    sf::RenderWindow window(sf::VideoMode(800, 600), "Multiple Balls in SFML");
    window.setFramerateLimit(60);

    //std::srand(static_cast<unsigned>(std::time(nullptr)));

    
    std::vector<Ball> balls;
    const int ballCount = 20;
    const float radius = 10.f;

    for (int i = 0; i < ballCount; ++i) {

        Ball ball;
        ball.shape.setRadius(radius);
        ball.shape.setFillColor(sf::Color(
            std::rand() % 256,
            std::rand() % 256,
            std::rand() % 256
        ));

        ball.shape.setPosition(
            static_cast<float>(std::rand() % (800 - static_cast<int>(2 * radius))),
            static_cast<float>(std::rand() % (600 - static_cast<int>(2 * radius)))
        );

        // Random velocity between -5 and 5
        ball.velocity = sf::Vector2f(
            static_cast<float>((std::rand() % 10) - 5),
            static_cast<float>((std::rand() % 10) - 5)
        );

        // Ensure velocity is not zero
        if (ball.velocity.x == 0) ball.velocity.x = 1;
        if (ball.velocity.y == 0) ball.velocity.y = 1;

        balls.push_back(ball);
    }

    // Main loop
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

        }

        // Update ball positions
        for (auto& ball : balls) {

            sf::Vector2f pos = ball.shape.getPosition();
            //pos += ball.velocity;

            // Bounce off walls
            if (pos.x <= 0 || pos.x + 2 * radius >= 800)
                ball.velocity.x = -ball.velocity.x*0.9;
            if (pos.y <= 0 || pos.y + 2 * radius >= 600)
                ball.velocity.y = -ball.velocity.y*0.9;

            ball.shape.move(ball.velocity);
            ball.velocity.y += 0.9f;

        }

        // Draw
        window.clear(sf::Color::Black);

        for (const auto& ball : balls) {

            window.draw(ball.shape);

        }
        window.display();
    }

    return 0;
}

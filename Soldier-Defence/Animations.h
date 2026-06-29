#pragma once

#include <SFML/Graphics.hpp>

class Animation
{
private:

    sf::Texture texture;
    sf::Sprite sprite;

    int frameWidth;
    int frameHeight;

    int totalFrames;
    int currentFrame;

    float animationTimer;
    float animationSpeed;

public:

    Animation();

    bool load(const std::string& filename,
        int frameWidth,
        int frameHeight,
        int totalFrames,
        float animationSpeed);

    void update(float deltaTime);

    sf::Sprite& getSprite();
}; 

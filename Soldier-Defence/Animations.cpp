#include "Animations.h"

Animation::Animation()
{
    currentFrame = 0;
    animationTimer = 0.f;
}

bool Animation::load(const std::string& filename,
    int frameWidth,
    int frameHeight,
    int totalFrames,
    float animationSpeed)
{
    if (!texture.loadFromFile(filename))
        return false;

    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;

    this->totalFrames = totalFrames;
    this->animationSpeed = animationSpeed;

    sprite.setTexture(texture);

    sprite.setTextureRect(
        sf::IntRect(0, 0, frameWidth, frameHeight)
    );

    return true;
}

void Animation::update(float deltaTime)
{
    animationTimer += deltaTime;

    if (animationTimer >= animationSpeed)
    {
        animationTimer = 0.f;

        currentFrame++;

        if (currentFrame >= totalFrames)
            currentFrame = 0;

        sprite.setTextureRect(
            sf::IntRect(
                currentFrame * frameWidth,
                0,
                frameWidth,
                frameHeight
            )
        );
    }
}

sf::Sprite& Animation::getSprite()
{
    return sprite;
}
#include "Animations.h"

Animation::Animation()
{
    frameWidth = 0;
    frameHeight = 0;
    totalFrames = 0;
    currentFrame = 0;

    animationTimer = 0.f;
    animationSpeed = 0.f;
}

Animation::Animation(Animation&& other) noexcept
{
    texture = std::move(other.texture);

    sprite.setTexture(texture);
    sprite.setTextureRect(other.sprite.getTextureRect());
    sprite.setPosition(other.sprite.getPosition());
    sprite.setScale(other.sprite.getScale());
    sprite.setRotation(other.sprite.getRotation());
    sprite.setOrigin(other.sprite.getOrigin());
    sprite.setColor(other.sprite.getColor());

    frameWidth = other.frameWidth;
    frameHeight = other.frameHeight;

    totalFrames = other.totalFrames;
    currentFrame = other.currentFrame;

    animationTimer = other.animationTimer;
    animationSpeed = other.animationSpeed;
}


Animation& Animation::operator=(Animation&& other) noexcept
{
    if (this != &other)
    {
        texture = std::move(other.texture);

        sprite.setTexture(texture);
        sprite.setTextureRect(other.sprite.getTextureRect());
        sprite.setPosition(other.sprite.getPosition());
        sprite.setScale(other.sprite.getScale());
        sprite.setRotation(other.sprite.getRotation());
        sprite.setOrigin(other.sprite.getOrigin());
        sprite.setColor(other.sprite.getColor());

        frameWidth = other.frameWidth;
        frameHeight = other.frameHeight;

        totalFrames = other.totalFrames;
        currentFrame = other.currentFrame;

        animationTimer = other.animationTimer;
        animationSpeed = other.animationSpeed;
    }

    return *this;
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
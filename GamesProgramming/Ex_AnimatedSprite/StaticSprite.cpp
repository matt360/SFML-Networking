#include "StaticSprite.h"

StaticSprite::StaticSprite(const sf::Vector2f & size) : Sprite(size)
{

}

StaticSprite::~StaticSprite()
{

}

void StaticSprite::update(float dt)
{
	// Do nothing as it is a static sprite.
	// Don't even need to call from main game loop.
}


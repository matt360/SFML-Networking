#include "Sprite.h"

Sprite::~Sprite()
{}

Sprite::Sprite(const sf::Vector2f & size) : RectangleShape(size)
{}

void Sprite::setVelocity(sf::Vector2f vel)
{
	velocity = vel;
}
void Sprite::setVelocity(float vx, float vy)
{
	velocity.x = vx;
	velocity.y = vy;
}

sf::Vector2f Sprite::getVelocity()
{
	return velocity;
}


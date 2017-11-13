#include "Ball.h"

Ball::Ball(const sf::Vector2f & size) : Sprite(size)
{	
}


Ball::~Ball()
{
}

void Ball::update(float dt)
{
	move(velocity*dt);
}
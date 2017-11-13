#include "Ball.h"

Ball::Ball(const sf::Vector2f & size) : Sprite(size)
{
	//max = 50f;
	falling = true;
	gravity = 0.5f;
}


Ball::~Ball()
{
}

void Ball::update(float dt)
{
	if (falling)
	{
		velocity.y += gravity;
		move(velocity*dt);
	}
	if (getPosition().y >= 500)
	{
		//falling = false;  // uncomment to stop bounce
		setPosition(getPosition().x, 500);
		velocity.y = (-velocity.y)/1.2f;
	}
	// if velocity is so low, set to zero, allow key press for jump
}
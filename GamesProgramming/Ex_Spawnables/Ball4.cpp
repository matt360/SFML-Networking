#include "Ball4.h"

Ball4::Ball4()
{
	velocity = sf::Vector2f(100, 100);
}

Ball4::~Ball4()
{
}

void Ball4::update(float dt)
{
	move(velocity*dt);
	if (getPosition().x < 0)
	{
		setPosition(0, getPosition().y);
		velocity.x = -velocity.x;
	}
	if (getPosition().x > 700)
	{
		setPosition(700, getPosition().y);
		velocity.x = -velocity.x;
	}
	if (getPosition().y < 0)
	{
		setPosition(getPosition().x, 0);
		velocity.y = -velocity.y;
	}
	if (getPosition().y > 500)
	{
		setPosition(getPosition().x, 500);
		velocity.y = -velocity.y;
	}
	updateAABB();
}

void Ball4::collisionResponse(Sprite* sp)
{
	velocity.y = -velocity.y;
}
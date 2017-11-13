#include "Square1.h"

Square1::Square1()
{
	setSize(sf::Vector2f(50, 50));
	setPosition(0, 200);
	updateAABB();
	setFillColor(sf::Color::Blue);
	setVelocity(50, 0);
}

Square1::~Square1()
{
}

void Square1::update(float dt)
{
	move(velocity*dt);

	if (getPosition().x < 0)
	{
		setPosition(0, getPosition().y);
		velocity.x = -velocity.x;
	}
	if (getPosition().x > 750)
	{
		setPosition(750, getPosition().y);
		velocity.x = -velocity.x;
	}
	updateAABB();
}
void Square1::collisionResponse(Sprite* sp)
{
	velocity.x = -velocity.x;
}
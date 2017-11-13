#include "Paddle.h"

Paddle::Paddle()
{
	velocity = sf::Vector2f(100, 0);
}

Paddle::~Paddle()
{
}

void Paddle::update(float dt)
{
	
	if (input->isKeyDown(sf::Keyboard::Right))
	{
		move(velocity*dt);
	}
	if (input->isKeyDown(sf::Keyboard::Left))
	{
	
		move(-velocity*dt);
	}
	updateAABB();
}

void Paddle::collisionResponse(Sprite* sp)
{
	//velocity.y = -velocity.y;
}
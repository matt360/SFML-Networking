#include "Ball3.h"



Ball3::Ball3()
{
	speed = 50.f;
	acceleration = 20.f;
	target = sf::Vector2f(600, 300);
	moving = true;
}


Ball3::~Ball3()
{
}

void Ball3::update(float dt)
{
	// calculate direction and move
	if (moving)
	{
		direction = target - getPosition();
		direction = Vector::normalise(direction);
		velocity = (direction * speed);
		move(velocity*dt);
	}

	// if object is close enough to taget
	if (Vector::magnitude(target - getPosition()) < 10.f)
	{
		moving = false;
		setPosition(target);
	}
	// increase speed
	//velocity.x += acceleration*dt;

	//if (velocity.x > speed)
	//{
		//velocity.x = speed;
	//}
	// reset object (position and velocity)
	if (input->isKeyDown(sf::Keyboard::Num1))
	{
		setPosition(0, 0);
		setVelocity(0, 0);
		moving = true;
	}
	if (input->isKeyDown(sf::Keyboard::Num2))
	{
		setPosition(0, 250);
		setVelocity(0, 0);
		moving = true;
	}
	if (input->isKeyDown(sf::Keyboard::Num3))
	{
		setPosition(0, 500);
		setVelocity(0, 0);
		moving = true;
	}
}
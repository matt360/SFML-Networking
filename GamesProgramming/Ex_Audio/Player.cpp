#include "Player.h"
#include <iostream>
Player::Player(const sf::Vector2f & size) : Sprite(size)
{
	falling = true;
}

Player::~Player()
{

}

void Player::update(float dt)
{
	if (falling)
	{
		//velocity.y += (1600.f)*dt;
		velocity.y += (1600.f);
	}

	if (input->isKeyDown(sf::Keyboard::Right))
	{
		velocity.x = 100;
	}
	else if (input->isKeyDown(sf::Keyboard::Left))
	{
		velocity.x = -100;
	}
	else
	{
		velocity.x = 0;
	}

	//if (input->isKeyDown(sf::Keyboard::Up))
	//{
	//	input->setKeyUp(sf::Keyboard::Up);
	//	velocity.y = -400.f;
	//	falling = true;
	//}

	//move(velocity*dt);
	move(velocity);
	updateAABB();
}

void Player::jump()
{
	velocity.y = -400.f;
	falling = true;
}

void Player::setInput(Input* in)
{
	input = in;
}

void Player::collisionRespone(Sprite* sp)
{
	// TODO dead response with the sprite working
	// std::cout << "dead" << std::endl;
	velocity.y = 0;
	setPosition(getPosition().x, sp->getPosition().y-getSize().y);
	//falling = false;
}


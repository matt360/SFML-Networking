#include "Player.h"
#include <iostream>
Player::Player(const sf::Vector2f & size) : Sprite(size)
{
	//falling = true;
	speed = 0.1;
}

Player::~Player()
{

}

void Player::update()
{
	//if (falling)
	//{
	//	//velocity.y += (1600.f)*dt;
	//	velocity.y += (1.0f);
	//}

	if (input->isKeyDown(sf::Keyboard::Right))
	{
		velocity.x += speed;
	}
	else if (input->isKeyDown(sf::Keyboard::Left))
	{
		velocity.x -= speed;
	}
	else if (input->isKeyDown(sf::Keyboard::Up))
	{
		velocity.y -= speed;
	}
	else if (input->isKeyDown(sf::Keyboard::Down))
	{
		velocity.y += speed;
	}
	else
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	//if (input->isKeyDown(sf::Keyboard::Up))
	//{
	//	input->setKeyUp(sf::Keyboard::Up);
	//	velocity.y = -.1f;
	//	falling = true;
	//}

	//move(velocity*dt);
	move(velocity);
	updateAABB();
}

void Player::jump()
{
	//velocity.y = -400.f;
	//falling = true;
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


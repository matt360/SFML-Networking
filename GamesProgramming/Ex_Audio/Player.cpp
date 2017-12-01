#include "Player.h"
#include <iostream>
Player::Player(const sf::Vector2f & size) : Sprite(size)
{
	speed = 0.1;
}

Player::~Player()
{

}

void Player::update()
{
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
	
	move(velocity);
	updateAABB();
}

void Player::setInput(Input* in)
{
	input = in;
}

void Player::collisionRespone(Sprite* sp)
{
	// TODO dead response with the sprite working
	std::cout << "dead\n";
	velocity.y = 0;
	setPosition(getPosition().x, sp->getPosition().y-getSize().y);
}


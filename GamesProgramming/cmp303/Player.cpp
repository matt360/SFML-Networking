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
	// Becasue the game is about showing networking there's no other consequences 
	// to touching the enemy than just seing the player's alive state set to false
	//velocity.y = 0;
	//velocity.x = 0;
	setAlive(false);
	setPosition(getPosition().x, sp->getPosition().y-getSize().y);
}


#include "Player.h"

Player::Player(const sf::Vector2f & size) : Sprite(size)
{

}

Player::~Player()
{

}

void Player::update(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Right))
	{
		move(velocity*dt);
	}

}

void Player::setInput(Input* in)
{
	input = in;
}


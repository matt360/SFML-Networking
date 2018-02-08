#include "Enemy.h"
#include <iostream>
Enemy::Enemy(const sf::Vector2f & size) : Sprite(size)
{
	speed = 0.08;
}

Enemy::~Enemy()
{

}

void Enemy::update()
{
	
	//if (input->isKeyDown(sf::Keyboard::Right))
	//{
	//	velocity.x += speed;
	//}
	//else if (input->isKeyDown(sf::Keyboard::Left))
	//{
	//	velocity.x -= speed;
	//}
	//else if (input->isKeyDown(sf::Keyboard::Up))
	//{
	//	velocity.y -= speed;
	//}
	//else if (input->isKeyDown(sf::Keyboard::Down))
	//{
	//	velocity.y += speed;
	//}
	//else
	//{
	//	velocity.x = 0;
	//	velocity.y = 0;
	//}

	move(velocity);
	updateAABB();
}

#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f & size) : Sprite(size)
{
	velocity.x = 100.f;
}

Enemy::~Enemy()
{}

void Enemy::update(float dt)
{
	move(velocity*dt);
}
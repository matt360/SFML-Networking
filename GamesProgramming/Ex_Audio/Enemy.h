#pragma once
#include "Sprite.h"

class Enemy : public Sprite
{
public:
	Enemy(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~Enemy();

	void update();

protected:
	float speed;
};
#pragma once
#include "Sprite.h"
#include "Input.h"
#include "Character.h"

class Enemy : public Sprite, public Character
{
public:
	Enemy(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~Enemy();

	void update();

protected:
	float speed;
};
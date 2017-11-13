#pragma once
#include "Sprite.h"

class Enemy : public Sprite
{
public:
	Enemy(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~Enemy();

	void update(float dt);
	//void handleInput(float dt);

protected:
	sf::Vector2f velocity;

};
#pragma once
#include "Sprite.h"

class StaticSprite : public Sprite
{
public:
	StaticSprite(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~StaticSprite();

	void update(float dt);

};
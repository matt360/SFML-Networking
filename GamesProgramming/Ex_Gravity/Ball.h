#pragma once
#include "Sprite.h"
class Ball : public Sprite
{
public:
	Ball(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~Ball();

	void update(float dt);

protected:
	const float max{ 50 };
	float gravity;
	bool falling;
};


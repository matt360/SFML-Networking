#pragma once
#include "Sprite.h"
#include "Input.h"

class Player : public Sprite
{
public :
	Player(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~Player();

	void update();

	void setInput(Input* in);
	void collisionRespone(Sprite* sp);

protected:
	Input* input;
	float speed;
	//bool falling;
};
#pragma once
#include "Sprite.h"
#include "Input.h"

class Cursor : public Sprite
{
public:
	Cursor(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~Cursor();

	void update(float dt);
	void setInput(Input* in);

protected:
	Input* input;
};
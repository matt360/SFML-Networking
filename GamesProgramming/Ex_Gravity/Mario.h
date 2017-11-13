#pragma once
#include "AnimatedSprite.h"
#include "Animation.h"
#include "Input.h"

class Mario : public AnimatedSprite
{
public:
	Mario(const sf::Vector2f &size = sf::Vector2f(0, 0));
	~Mario();

	void update(float dt);
	void setInput(Input* in) { input = in; };

protected:
	Animation walk;
	Animation swim;
	Animation duck;
	Animation* currentAnimation;
	Input* input;
};
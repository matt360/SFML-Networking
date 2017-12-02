#pragma once
#include "Sprite.h"

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(const sf::Vector2f &size = sf::Vector2f(0, 0));
	~AnimatedSprite();

	void update(float dt);

	void setFrameSize(int width, int height);
	int getFrameWidth() { return frame.width; };
	int getFrameHeight() { return frame.height; };
	void setAnimationSpeed(float aspeed);

protected:
	// variable for controlling animation
	float frameTime;
	float elapsedTime;
	float animationSpeed;
	sf::IntRect frame;

};
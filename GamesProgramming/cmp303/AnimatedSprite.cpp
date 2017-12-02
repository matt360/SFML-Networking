#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(const sf::Vector2f &size) : Sprite(size)
{
	// Configure default values
	elapsedTime = 0.f;
	animationSpeed = 1.0f;
	frame = sf::IntRect(0, 0, 0, 0);
}

AnimatedSprite::~AnimatedSprite()
{}

void AnimatedSprite::update(float dt)
{
	// increment time
	elapsedTime += dt;

	// if enough time has passed move onto next frame
	if (elapsedTime >= animationSpeed)
	{
		frame.left += frame.width;
		// check if we have passed last frame of animation
		if (frame.left > getTexture()->getSize().x - frame.width)
		{
			frame.left = 0;
		}
		setTextureRect(frame);
		// reset timer
		elapsedTime = 0;
		
	}
}

void AnimatedSprite::setAnimationSpeed(float speed)
{
	animationSpeed = speed;
}

void AnimatedSprite::setFrameSize(int width, int height)
{
	frame.width = width;
	frame.height = height;
	setTextureRect(frame);
}
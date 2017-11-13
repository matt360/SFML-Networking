#include "Animation.h"

Animation::Animation()
{
	currentFrame = 0;
}

Animation::~Animation()
{}

void Animation::init(int left, int top, int width, int height, int numFrames, float speed)
{
	frame.left = left;
	frame.top = top;
	frame.width = width;
	frame.height = height;
	numberOfFrames = numFrames;
	animationSpeed = speed;
}

sf::IntRect Animation::getCurrentFrame()
{
	sf::IntRect temp;
	if (flipped)
	{
		temp = sf::IntRect((frame.width * currentFrame) + frame.width, frame.top, -frame.width, frame.height);
	}
	else
	{
		temp = sf::IntRect(frame.width * currentFrame, frame.top, frame.width, frame.height);
	}
	//sf::IntRect temp(frame.width * currentFrame, frame.top, frame.width, frame.height);
	//sf::IntRect temp((frame.width * currentFrame)+frame.width, frame.top, -frame.width, frame.height);
	return temp;
}

void Animation::nextFrame()
{
	currentFrame++;
	if (currentFrame >= numberOfFrames)
	{
		currentFrame = 0;
	}
}
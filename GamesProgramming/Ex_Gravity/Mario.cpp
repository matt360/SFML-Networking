#include "Mario.h"

Mario::Mario(const sf::Vector2f &size) : AnimatedSprite(size)
{
	walk.init(0, 0, 15, 21, 4, 0.3f);
	swim.init(0, 21, 16, 20, 3, 0.2f);
	duck.init(0, 41, 16, 20, 2, 1.f);
	currentAnimation = &walk;
	frame = currentAnimation->getCurrentFrame();
	setTextureRect(frame);
}

Mario::~Mario()
{

}

void Mario::update(float dt)
{
	elapsedTime += dt;
	if (elapsedTime >= currentAnimation->getAnimationTime())
	{
		// next frame
		currentAnimation->nextFrame();
		setTextureRect(currentAnimation->getCurrentFrame());
		elapsedTime = 0;
	}

	if (input->isKeyDown(sf::Keyboard::Num1))
	{
		currentAnimation = &walk;
		elapsedTime = 0;
		setTextureRect(currentAnimation->getCurrentFrame());
	}
	if (input->isKeyDown(sf::Keyboard::Num2))
	{
		currentAnimation = &swim;
		elapsedTime = 0;
		setTextureRect(currentAnimation->getCurrentFrame());
	}
	if (input->isKeyDown(sf::Keyboard::Num3))
	{
		currentAnimation = &duck;
		elapsedTime = 0;
		setTextureRect(currentAnimation->getCurrentFrame());
	}
	if (input->isKeyDown(sf::Keyboard::Q))
	{
		input->setKeyUp(sf::Keyboard::Q);
		currentAnimation->setFlipped(!currentAnimation->getFlipped());
		setTextureRect(currentAnimation->getCurrentFrame());
	}
}
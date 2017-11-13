#include "Sprite.h"

Sprite::Sprite()
{
	// Load texture
	texture.loadFromFile("gfx/MushroomTrans.png");
	// apply texture to sprite object
	sprite.setTexture(texture);

	sprite.setPosition(200, 200);
	//sprite.setTextureRect(sf::IntRect(10, 10, 32, 32));
	sprite.setScale(sf::Vector2f(0.5, 0.5f));
}

Sprite::~Sprite()
{

}

void Sprite::update(float dt)
{
	// Update for sprite
	// Self-contained movement etc
}

void Sprite::render(sf::RenderWindow* window)
{
	window->draw(sprite);
}
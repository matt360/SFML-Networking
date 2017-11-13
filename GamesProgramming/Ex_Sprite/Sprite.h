#pragma once
#include <SFML\Graphics.hpp>
// Sprite class needs to track, position, load image, could handle input. Fairly simple class, but will be built on for animated sprite.

class Sprite
{
public:
	Sprite();
	~Sprite();

	void update(float dt);
	void render(sf::RenderWindow* window);

//protected:
	sf::Sprite sprite;
	sf::Texture texture;
};
#pragma once
#include "SFML\Graphics.hpp"

class Sprite : public sf::RectangleShape
{
public:
	Sprite(const sf::Vector2f & size = sf::Vector2f(0, 0));
	~Sprite();

	virtual void update() = 0;
	void setVelocity(sf::Vector2f vel);
	void setVelocity(float vx, float vy);
	sf::Vector2f getVelocity();
	
	sf::FloatRect getAABB();
	virtual void updateAABB();
	virtual void collisionResponse();
	
	void setAlive(bool live) { alive = live; };
	bool isAlive() { return alive; };

protected:
	sf::Vector2f velocity;
	sf::FloatRect AABB;
	bool alive;
};
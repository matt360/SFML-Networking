#pragma once
#include "Ball2.h"
#include "Vector.h"

class Ball3 : public Ball2
{
public:
	Ball3();
	~Ball3();

	void update(float dt);
	void setAcceleration(float acc) { acceleration = acc; };
	float getAcceleration() { return acceleration; };

protected:
	float acceleration;
	float speed;
	sf::Vector2f direction;
	sf::Vector2f target;
	bool moving;
};


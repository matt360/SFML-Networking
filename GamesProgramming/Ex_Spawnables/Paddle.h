#pragma once
#include "Sprite.h"
#include "Player.h"
class Paddle : public Player
{
public:
	Paddle();
	~Paddle();

	void update(float dt);
	void collisionResponse(Sprite* sp);
};


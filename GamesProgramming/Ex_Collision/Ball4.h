#pragma once
#include "Sprite.h"
class Ball4 : public Sprite
{
public:
	Ball4();
	~Ball4();

	void update(float dt);
	void collisionResponse(Sprite* sp);
};


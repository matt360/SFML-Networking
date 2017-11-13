#pragma once
#include "Sprite.h"
class Square1 :
	public Sprite
{
public:
	Square1();
	~Square1();

	void update(float dt);
	void collisionResponse(Sprite* sp);
};


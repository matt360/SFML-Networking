#pragma once
#include "Sprite.h"

class Character
{
public:
	Character();
	~Character();

	virtual void update() = 0;
};


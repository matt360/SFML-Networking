#pragma once
#include "Player.h"
#include "Ball.h"

class Ball2 : public Player
{
public:
	Ball2();
	~Ball2();

	void update(float);

protected:
	float gravity;
	bool falling;
	float scale;
};


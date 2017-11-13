#include "Ball2.h"

Ball2::Ball2()
{
	scale = 200.f;
	gravity = 8.0f*scale;
	falling = true;
}


Ball2::~Ball2()
{
}

void Ball2::update(float dt)
{
	if (falling)
	{
		//float temp = gravity *dt;
		velocity.y += (gravity)*dt;///**gravity*/)*dt;
		//if (velocity.y > 50*scale)
			//velocity.y = 50*scale;
		move(velocity*dt);
	}
	if (getPosition().y >= 500)
	{
		falling = false;  // uncomment to stop bounce
		setPosition(getPosition().x, 500);
		//velocity.y = (-velocity.y) / 1.2f;
	}
	
	// if velocity is so low, set to zero, allow key press for jump
	if (input->isKeyDown(sf::Keyboard::Space))
	{
		velocity.y = -2.f*scale;
		falling = true;
	}
}
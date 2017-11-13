#pragma once
#include "Ball.h"
#include <math.h>
#include <vector>
#include <list>

class BeachBallManager
{
public:
	BeachBallManager();
	~BeachBallManager();

	void spawn();
	void update(float dt);
	void deathCheck();
	void render(sf::RenderWindow* window);

private:
	std::vector<Ball> balls;
	//std::list<Ball> balls;
	//Ball balls[20];
	sf::Vector2f spawnPoint;
	sf::Texture texture;
};


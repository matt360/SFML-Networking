#pragma once
#include "SFML\Graphics.hpp"

class Animation
{
public:
	Animation();
	~Animation();

	void init(int left, int top, int width, int height, int frames, float speed);

	sf::IntRect getCurrentFrame();
	float getAnimationTime() { return animationSpeed; };
	void nextFrame();
	void setFlipped(bool flip) { flipped = flip; };
	bool getFlipped() {	return flipped;	};

	// could add functionality for play, pause, stop and loop

protected:
	sf::IntRect frame;
	int numberOfFrames;
	int currentFrame;
	float animationSpeed;
	bool flipped;
};
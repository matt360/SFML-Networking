#pragma once
#include <SFML/System/Vector2.hpp>

class Prediction 
{
public:
	Prediction();
	~Prediction();

protected:
	inline float lerp(float start, float end, float time);
	sf::Vector2f lerp(const sf::Vector2f & start, const sf::Vector2f& end, const float& time);
};

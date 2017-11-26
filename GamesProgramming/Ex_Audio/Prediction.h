#pragma once
#include <SFML/System.hpp>

class Prediction 
{
public:
	Prediction();
	~Prediction();

protected:
	sf::Int32 getCurrentTime(const sf::Clock& clock, const sf::Int32& offset);

	inline float lerp(float start, float end, float time);
	sf::Vector2f lerp(const sf::Vector2f & start, const sf::Vector2f& end, const float& time);
};

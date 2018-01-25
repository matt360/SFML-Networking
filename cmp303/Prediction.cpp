#include "Prediction.h"

Prediction::Prediction() {}

Prediction::~Prediction() {}

inline float Prediction::lerp(float start, float end, float time)
{
	return start * (1.0f - time) + time * end;
}

sf::Vector2f Prediction::lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float& time)
{
	sf::Vector2f temp;
	temp.x = lerp(start.x, end.x, time);
	temp.y = lerp(start.y, end.y, time);
	return temp;
}
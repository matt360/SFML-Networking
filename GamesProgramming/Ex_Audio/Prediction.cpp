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

sf::Int32 Prediction::getCurrentTime(const sf::Clock& clock, const sf::Int32& offset)
{
	sf::Int32 current_time = clock.getElapsedTime().asMilliseconds();
	return sf::Int32(current_time + (offset));
}
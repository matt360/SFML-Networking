#pragma once
#include <SFML/Network.hpp>
#include "Player.h"
#include "Messages.h"
#include <queue>
#include <deque>

class QuadraticPrediction
{
public:
	QuadraticPrediction();
	~QuadraticPrediction();

protected:
	float lerp(float start, float end, float time);
	sf::Vector2f lerp(const sf::Vector2f & start, const sf::Vector2f& end, const float& time);
	sf::Int32 getCurrentTime(const sf::Clock& clock, const sf::Int32& offset);

	// number of messages to keep track of
	const unsigned int quadratic_message_number = 3;
	std::deque<PlayerMessage> quadratic_local_positions;
	void keepTrackOfQuadraticLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfQuadraticLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfQuadraticNetworkPositions(const PlayerMessage& player_message_receive);
	std::deque<PlayerMessage> quadratic_network_positions;
	sf::Vector2f predictQuadraticLocalPath(const sf::Clock& clock, const sf::Int32& offset);
	sf::Vector2f predictQuadraticNetworkPath(const sf::Clock& clock, const sf::Int32& offset);

	void quadraticInterpolation(Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& lerp_mode);
};


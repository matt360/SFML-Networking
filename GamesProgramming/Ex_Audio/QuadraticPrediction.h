#pragma once
#include <SFML/Network.hpp>
#include "Player.h"
#include "Messages.h"
#include "Prediction.h"
#include <queue>
#include <deque>

class QuadraticPrediction : public Prediction
{
public:
	QuadraticPrediction();
	~QuadraticPrediction();

protected:
	using Prediction::getCurrentTime;

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


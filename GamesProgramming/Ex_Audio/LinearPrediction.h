#pragma once
#include <SFML/Network.hpp>
#include "Player.h"
#include "Messages.h"
#include "Prediction.h"
#include <queue>
#include <deque>

class LinearPrediction : public Prediction
{
public:
	LinearPrediction();
	~LinearPrediction();

protected:
	// number of messages to keep track of
	const unsigned int linear_message_number = 2;
	std::queue<PlayerMessage> linear_local_positions;

	void keepTrackOfLinearLocalPositoins(const Sprite& player, const sf::Int32& time);
	void keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Int32& time);
	void keepTrackOfLinearNetworkPositions(const PlayerMessage& player_message_receive);

	std::queue<PlayerMessage> linear_network_positions;
	sf::Vector2f predictLinearLocalPath(const sf::Int32& time);
	sf::Vector2f predictLinearNetworkPath(const sf::Int32& time);

	void linearInterpolation(Sprite& player, const sf::Int32& time, const bool& lerp_mode);
};


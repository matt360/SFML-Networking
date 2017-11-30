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

	const unsigned int linear_message_number = 2;

	std::queue<PlayerMessage> linear_local_positions;
	std::queue<PlayerMessage> linear_network_positions;

	void keepTrackOfLinearLocalPositoins(const Sprite& player, const sf::Int32& time);
	void keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Int32& time);
	void keepTrackOfLinearNetworkPositions(const PlayerMessage& player_message_receive);

	void linearInterpolation(Sprite& player, const sf::Int32& time, const bool& lerp_mode);
protected:
	// number of messages to keep track of

	sf::Vector2f predictLinearLocalPath(const sf::Int32& time);
	sf::Vector2f predictLinearNetworkPath(const sf::Int32& time);
};


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

	// history of sprite's local positions
	std::queue<Message> linear_local_positions;
	// history of sprite's network positions
	std::queue<Message> linear_network_positions;

	//void keepTrackOfLinearLocalPositoins(const Sprite& sprite, const sf::Int32& time);
	// for adding lepred position to the history of positions
	void keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Int32& time);

	void keepTrackOfLinearLocalPositoins(const Message & local_message);
	void keepTrackOfLinearNetworkPositions(const Message& message_receive);

	//void keepTrackOfLinearNetworkPositions(sf::Vector2f & vec, const sf::Int32& tm);

	sf::Vector2f predictLinearLocalPath(std::queue<Message> history_of_local_positions, const sf::Int32& time);

	sf::Vector2f predictLinearNetworkPath(std::queue<Message> history_of_network_positions, const sf::Int32& time);

	void linearInterpolation(Sprite& sprite, const sf::Int32& time, const bool& lerp_mode);
protected:
	// number of messages to keep track of

	sf::Vector2f predictLinearLocalPath(std::queue<Message> history_of_local_positions, const sf::Int32& time);
	sf::Vector2f predictLinearNetworkPath(std::queue<Message> history_of_network_positions, const sf::Int32& time);
};


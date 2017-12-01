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

	const unsigned int quadratic_message_number = 3;

	std::deque<Message> local_message_history;
	std::deque<Message> network_message_history;

	void keepTrackOfQuadraticLocalPositoins(const Message& local_message);
	void keepTrackOfQuadraticNetworkPositions(const Message& message_receive);

	void quadraticInterpolation(Sprite& player, const sf::Int32& tm, const bool& lerp_mode);
protected:
	// number of messages to keep track of
	sf::Vector2f predictQuadraticLocalPath(const sf::Int32& tm);
	sf::Vector2f predictQuadraticNetworkPath(const sf::Int32& tm);
};


#pragma once
#include <SFML/Network.hpp>
#include "Sprite.h"
#include "Messages.h"
#include "Prediction.h"
#include <queue>
#include <deque>

class LinearPrediction : public Prediction
{
public:
	LinearPrediction();
	~LinearPrediction();

	// number of messages to keep track of
	const unsigned int linear_message_number = 2;

	// history of messages used for linear interpolation
	std::queue<Message> local_message_history;
	// history of sprite's network positions
	std::queue<Message> network_message_history;

	void keepTrackOfLinearLocalPositoins(const Message& local_message);
	void keepTrackOfLinearNetworkPositions(const Message& message_receive);

	sf::Vector2f linearInterpolation(Sprite & sprite, sf::Vector2f & msg0_local_position, sf::Vector2f & masg1_local_position, sf::Vector2f & msg0_network_position, sf::Vector2f & msg1_network_position, float & msg0_time, float & msg1_time, const sf::Int32 & tm, const bool & lerp_mode);
protected:
	sf::Vector2f predictLinearLocalPath(sf::Vector2f & msg0_local_position, sf::Vector2f & masg1_local_position, float & msg0_time, float & msg1_time, float & time);
	sf::Vector2f predictLinearNetworkPath(sf::Vector2f & msg0_network_position, sf::Vector2f & msg1_network_position, float & time0, float & time1, float & time);
	
};


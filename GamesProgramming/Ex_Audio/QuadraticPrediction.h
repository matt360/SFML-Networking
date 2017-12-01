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

	// number of messages to keep track of
	const unsigned int quadratic_message_number = 3;

	std::deque<Message> local_message_history;
	std::deque<Message> network_message_history;

	void keepTrackOfQuadraticLocalPositoins(const Message& local_message);
	void keepTrackOfQuadraticNetworkPositions(const Message& message_receive);

	void quadraticInterpolation(Sprite & sprite, 
		sf::Vector2f & msg0_local_position, 
		sf::Vector2f & msg1_local_position, 
		sf::Vector2f & msg2_local_position,
		sf::Vector2f & msg0_network_position, 
		sf::Vector2f & msg1_network_position, 
		sf::Vector2f & msg2_network_position,
		float &msg0_time,
		float & msg1_time, 
		float & msg2_time,
		const sf::Int32 & tm, 
		const bool & lerp_mode);
protected:
	sf::Vector2f predictQuadraticLocalPath(sf::Vector2f & msg0_local_position, 
		sf::Vector2f & msg1_local_position, 
		sf::Vector2f & msg2_local_position,
		float &msg0_time, 
		float & msg1_time, 
		float & msg2_time,
		float &time);
	sf::Vector2f predictQuadraticNetworkPath(sf::Vector2f & msg0_network_position, 
		sf::Vector2f & msg1_network_position, 
		sf::Vector2f & msg2_network_position,
		float &msg0_time, 
		float & msg1_time, 
		float & msg2_time,
		float &time);
};


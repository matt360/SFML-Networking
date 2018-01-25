#include "LinearPrediction.h"

LinearPrediction::LinearPrediction() {}

LinearPrediction::~LinearPrediction() {}

void LinearPrediction::keepTrackOfLinearLocalPositoins(const Message& local_message)
{
	if (local_message_history.size() >= linear_message_number) local_message_history.pop();
	local_message_history.push(local_message);
}

void LinearPrediction::keepTrackOfLinearNetworkPositions(const Message& message_receive)
{
	if (network_message_history.size() >= linear_message_number) network_message_history.pop();
	network_message_history.push(message_receive);
}

sf::Vector2f LinearPrediction::predictLinearLocalPath(sf::Vector2f& msg0_local_position, sf::Vector2f& msg1_local_position, 
	float& msg0_time, float& msg1_time, float& time)
{
	float x_average_velocity, y_average_velocity;

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity = (msg0_local_position.x - msg1_local_position.x) / (msg0_time - msg1_time);
	y_average_velocity = (msg0_local_position.y - msg1_local_position.y) / (msg0_time - msg1_time);

	//// linear model
	float x_, y_;
	x_ = x_average_velocity * (time - msg1_time) + msg1_local_position.x;
	y_ = y_average_velocity * (time - msg1_time) + msg1_local_position.y;

	sf::Vector2f local_player_pos(x_, y_);
	return local_player_pos;
}

sf::Vector2f LinearPrediction::predictLinearNetworkPath(sf::Vector2f& msg0_network_position, sf::Vector2f& msg1_network_position,
	float& msg0_time, float& msg1_time, float& time)
{
	float x_average_velocity, y_average_velocity, x_, y_;

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity = (msg0_network_position.x - msg1_network_position.x) / (msg0_time - msg1_time);
	y_average_velocity = (msg0_network_position.y - msg1_network_position.y) / (msg0_time - msg1_time);

	// linear model
	x_ = x_average_velocity * (time - msg1_time) + msg1_network_position.x;
	y_ = y_average_velocity * (time - msg1_time) + msg1_network_position.y;

	sf::Vector2f network_player_pos(x_, y_);
	return network_player_pos;
}

// pass local position vectors and network position vectors of the sprite
sf::Vector2f LinearPrediction::linearInterpolation(Sprite& sprite, 
	sf::Vector2f& msg0_local_position, 
	sf::Vector2f& msg1_local_position,
	sf::Vector2f& msg0_network_position, 
	sf::Vector2f& msg1_network_position,
	float& msg0_time,
	float& msg1_time,
	const sf::Int32& tm, 
	const bool& lerp_mode)
{
	float time = (float)tm;
	// TODO
	sf::Vector2f local_path = predictLinearLocalPath(msg0_local_position,
		msg1_local_position, 
		msg0_time,
		msg1_time, 
		time);
	sf::Vector2f network_path = predictLinearNetworkPath(msg0_network_position,
		msg1_network_position,
		msg0_time, 
		msg1_time, 
		time);
	//lerp path works better with 100ms lag
	sf::Vector2f lerp_position = lerp(local_path, network_path, 0.1);

	// set player_position
	lerp_mode ? sprite.setPosition(lerp_position) : sprite.setPosition(network_path);

	// add lerped to the history of the local posistions
	return lerp_position;
}


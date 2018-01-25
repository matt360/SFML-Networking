#include "QuadraticPrediction.h"

QuadraticPrediction::QuadraticPrediction()
{
}


QuadraticPrediction::~QuadraticPrediction()
{
}

void QuadraticPrediction::keepTrackOfQuadraticLocalPositoins(const Message& local_message)
{ 
	if (local_message_history.size() >= quadratic_message_number) local_message_history.pop_back();
	local_message_history.push_front(local_message);
}

void QuadraticPrediction::keepTrackOfQuadraticNetworkPositions(const Message& message_receive)
{
	if (network_message_history.size() >= quadratic_message_number) network_message_history.pop_back();
	network_message_history.push_front(message_receive);
}

sf::Vector2f QuadraticPrediction::predictQuadraticLocalPath(
	sf::Vector2f & msg0_local_position,
	sf::Vector2f & msg1_local_position,
	sf::Vector2f & msg2_local_position,
	float & msg0_time,
	float & msg1_time,
	float & msg2_time,
	float &time)
{
	// quadratic model
	float x_average_velocity_1, y_average_velocity_1,
		x_average_velocity_2, y_average_velocity_2,
		a_x, a_y,
		x_, y_;

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity_1 = (msg0_local_position.x - msg1_local_position.x) / (msg0_time - msg1_time);
	y_average_velocity_1 = (msg0_local_position.y - msg1_local_position.y) / (msg0_time - msg1_time);

	y_average_velocity_2 = (msg1_local_position.y - msg2_local_position.y) / (msg1_time - msg2_time);
	x_average_velocity_2 = (msg1_local_position.x - msg2_local_position.x) / (msg1_time - msg2_time);

	a_x = (x_average_velocity_2 - x_average_velocity_1) / (msg2_time - msg0_time);
	a_y = (y_average_velocity_2 - y_average_velocity_1) / (msg2_time - msg0_time);

	// s = s0 + v0t + ½at2
	x_ = msg2_local_position.x + (x_average_velocity_2 * (time - msg2_time)) + ((0.5 * a_x) * powf((time - msg2_time), 2));
	y_ = msg2_local_position.y + (y_average_velocity_2 * (time - msg2_time)) + ((0.5 * a_y) * powf((time - msg2_time), 2));

	sf::Vector2f local_player_pos(x_, y_);
	return local_player_pos;
}

sf::Vector2f QuadraticPrediction::predictQuadraticNetworkPath(
	sf::Vector2f & msg0_network_position,
	sf::Vector2f & msg1_network_position,
	sf::Vector2f & msg2_network_position,
	float & msg0_time,
	float & msg1_time,
	float & msg2_time,
	float &time)
{
	// quadratic model
	float x_average_velocity_1, y_average_velocity_1,
		x_average_velocity_2, y_average_velocity_2,
		a_x, a_y,
		x_, y_;

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity_1 = (msg0_network_position.x - msg1_network_position.x) / (msg0_time - msg1_time);
	y_average_velocity_1 = (msg0_network_position.y - msg1_network_position.y) / (msg0_time - msg1_time);

	x_average_velocity_2 = (msg1_network_position.x - msg2_network_position.x) / (msg1_time - msg2_time);
	y_average_velocity_2 = (msg1_network_position.y - msg2_network_position.y) / (msg1_time - msg2_time);

	a_x = (x_average_velocity_2 - x_average_velocity_1) / (msg2_time - msg0_time);
	a_y = (y_average_velocity_2 - y_average_velocity_1) / (msg2_time - msg0_time);

	// s = s0 + v0t + ½at2
	x_ = msg2_network_position.x + (x_average_velocity_2 * (time - msg2_time)) + ((0.5 * a_x) * powf((time - msg2_time), 2));
	y_ = msg2_network_position.y + (y_average_velocity_2 * (time - msg2_time)) + ((0.5 * a_y) * powf((time - msg2_time), 2));

	sf::Vector2f network_player_pos(x_, y_);
	return network_player_pos;
}

sf::Vector2f QuadraticPrediction::quadraticInterpolation(Sprite& sprite,
	sf::Vector2f & msg0_local_position,
	sf::Vector2f & msg1_local_position,
	sf::Vector2f & msg2_local_position,
	sf::Vector2f & msg0_network_position,
	sf::Vector2f & msg1_network_position,
	sf::Vector2f & msg2_network_position,
	float & msg0_time,
	float & msg1_time,
	float & msg2_time,
	const sf::Int32 & tm,
	const bool & lerp_mode)
{
	float time = (float)tm;

	sf::Vector2f local_path = predictQuadraticLocalPath(
		msg0_local_position, 
		msg1_local_position, 
		msg2_local_position, 
		msg0_time, 
		msg1_time, 
		msg2_time, 
		time);
	sf::Vector2f network_path = predictQuadraticNetworkPath(msg0_network_position, 
		msg1_network_position,
		msg2_network_position,
		msg0_time, 
		msg1_time, 
		msg2_time,
		time);
	//lerp path works better with 100ms lag
	sf::Vector2f lerp_position = lerp(local_path, network_path, 0.1);

	// set player_position
	lerp_mode ? sprite.setPosition(lerp_position) : sprite.setPosition(network_path);

	// add lerped to the history of the local posistions
	return lerp_position;
}
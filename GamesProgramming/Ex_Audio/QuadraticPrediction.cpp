#include "QuadraticPrediction.h"

QuadraticPrediction::QuadraticPrediction()
{
}


QuadraticPrediction::~QuadraticPrediction()
{
}

void QuadraticPrediction::keepTrackOfQuadraticLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset)
{
	// local message
	PlayerMessage local_message;
	local_message.position.x = player.getPosition().x;
	local_message.position.y = player.getPosition().y;
	local_message.time = (float)getCurrentTime(clock, offset);
	//
	if (quadratic_local_positions.size() >= quadratic_message_number) quadratic_local_positions.pop_back();
	quadratic_local_positions.push_front(local_message);
}

void QuadraticPrediction::keepTrackOfQuadraticLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset)
{
	// local message
	PlayerMessage local_message;
	local_message.position.x = vec.x;
	local_message.position.y = vec.y;
	local_message.time = (float)getCurrentTime(clock, offset);
	// 
	if (quadratic_local_positions.size() >= quadratic_message_number) quadratic_local_positions.pop_back();
	quadratic_local_positions.push_front(local_message);
}

void QuadraticPrediction::keepTrackOfQuadraticNetworkPositions(const PlayerMessage& player_message_receive)
{
	if (quadratic_network_positions.size() >= quadratic_message_number) quadratic_network_positions.pop_back();
	quadratic_network_positions.push_front(player_message_receive);
}

sf::Vector2f QuadraticPrediction::predictQuadraticLocalPath(const sf::Clock& clock, const sf::Int32& offset)
{
	// quadratic model
	float x_average_velocity_1, y_average_velocity_1,
		x_average_velocity_2, y_average_velocity_2,
		a_x, a_y,
		x_, y_;


	PlayerMessage msg0 = quadratic_network_positions.at(0);
	PlayerMessage msg1 = quadratic_network_positions.at(1);
	PlayerMessage msg2 = quadratic_network_positions.at(2);
	float time = (float)getCurrentTime(clock, offset);

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity_1 = (msg0.position.x - msg1.position.x) / (msg0.time - msg1.time);
	y_average_velocity_1 = (msg0.position.y - msg1.position.y) / (msg0.time - msg1.time);

	x_average_velocity_2 = (msg1.position.x - msg2.position.x) / (msg1.time - msg2.time);
	y_average_velocity_2 = (msg1.position.y - msg2.position.y) / (msg1.time - msg2.time);

	a_x = (x_average_velocity_2 - x_average_velocity_1) / (msg2.time - msg0.time);
	a_y = (y_average_velocity_2 - y_average_velocity_1) / (msg2.time - msg0.time);

	// s = s0 + v0t + ½at2
	x_ = msg2.position.x + (x_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_x) * powf((time - msg2.time), 2));
	y_ = msg2.position.y + (y_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_y) * powf((time - msg2.time), 2));

	sf::Vector2f local_player_pos(x_, y_);
	return local_player_pos;
}

sf::Vector2f QuadraticPrediction::predictQuadraticNetworkPath(const sf::Clock& clock, const sf::Int32& offset)
{
	// quadratic model
	float x_average_velocity_1, y_average_velocity_1,
		x_average_velocity_2, y_average_velocity_2,
		a_x, a_y,
		x_, y_;


	PlayerMessage msg0 = quadratic_network_positions.at(0);
	PlayerMessage msg1 = quadratic_network_positions.at(1);
	PlayerMessage msg2 = quadratic_network_positions.at(2);
	float time = (float)getCurrentTime(clock, offset);

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity_1 = (msg0.position.x - msg1.position.x) / (msg0.time - msg1.time);
	y_average_velocity_1 = (msg0.position.y - msg1.position.y) / (msg0.time - msg1.time);

	x_average_velocity_2 = (msg1.position.x - msg2.position.x) / (msg1.time - msg2.time);
	y_average_velocity_2 = (msg1.position.y - msg2.position.y) / (msg1.time - msg2.time);

	a_x = (x_average_velocity_2 - x_average_velocity_1) / (msg2.time - msg0.time);
	a_y = (y_average_velocity_2 - y_average_velocity_1) / (msg2.time - msg0.time);

	// s = s0 + v0t + ½at2
	x_ = msg2.position.x + (x_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_x) * powf((time - msg2.time), 2));
	y_ = msg2.position.y + (y_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_y) * powf((time - msg2.time), 2));

	sf::Vector2f network_player_pos(x_, y_);
	return network_player_pos;
}

void QuadraticPrediction::quadraticInterpolation(Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& lerp_mode)
{
	sf::Vector2f local_path = predictQuadraticLocalPath(clock, offset);
	sf::Vector2f network_path = predictQuadraticNetworkPath(clock, offset);
	//lerp path works better with 100ms lag
	sf::Vector2f lerp_position = lerp(local_path, network_path, 0.1);

	// set position
	lerp_mode ? player.setPosition(lerp_position) : player.setPosition(network_path);

	// add lerped to the history of the local posistions
	keepTrackOfQuadraticLocalPositoins(lerp_position, clock, offset);
}
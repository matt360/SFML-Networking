#include "LinearPrediction.h"

LinearPrediction::LinearPrediction()
{
}


LinearPrediction::~LinearPrediction()
{
}

//sf::Int32 LinearPrediction::getCurrentTime(const sf::Clock& clock, const sf::Int32& offset)
//{
//	sf::Int32 current_time = clock.getElapsedTime().asMilliseconds();
//	return sf::Int32(current_time + (offset));
//}

void LinearPrediction::keepTrackOfLinearLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset)
{
	// local message
	PlayerMessage local_message;
	local_message.position.x = player.getPosition().x;
	local_message.position.y = player.getPosition().y;
	local_message.time = (float)getCurrentTime(clock, offset);

	if (linear_local_positions.size() >= linear_message_number) linear_local_positions.pop();
	linear_local_positions.push(local_message);
}

void LinearPrediction::keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset)
{
	// local message
	PlayerMessage local_message;
	local_message.position.x = vec.x;
	local_message.position.y = vec.y;
	local_message.time = (float)getCurrentTime(clock, offset);
	// 
	if (linear_local_positions.size() >= linear_message_number) linear_local_positions.pop();
	linear_local_positions.push(local_message);
}

void LinearPrediction::keepTrackOfLinearNetworkPositions(const PlayerMessage& player_message_receive)
{
	if (linear_network_positions.size() >= linear_message_number) linear_network_positions.pop();
	linear_network_positions.push(player_message_receive);
}

sf::Vector2f LinearPrediction::predictLinearLocalPath(const sf::Clock& clock, const sf::Int32& offset)
{
	float x_average_velocity, y_average_velocity;
	PlayerMessage msg0 = linear_local_positions.front();
	PlayerMessage msg1 = linear_local_positions.back();
	float time = (float)getCurrentTime(clock, offset);

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity = (msg0.position.x - msg1.position.x) / (msg0.time - msg1.time);
	y_average_velocity = (msg0.position.y - msg1.position.y) / (msg0.time - msg1.time);

	//// linear model
	float x_, y_;
	x_ = x_average_velocity * (time - msg1.time) + msg1.position.x;
	y_ = y_average_velocity * (time - msg1.time) + msg1.position.y;

	sf::Vector2f local_player_pos(x_, y_);
	return local_player_pos;
}

sf::Vector2f LinearPrediction::predictLinearNetworkPath(const sf::Clock& clock, const sf::Int32& offset)
{
	float x_average_velocity, y_average_velocity, x_, y_;
	PlayerMessage msg0 = linear_network_positions.front();
	PlayerMessage msg1 = linear_network_positions.back();
	float time = (float)getCurrentTime(clock, offset);

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity = (msg0.position.x - msg1.position.x) / (msg0.time - msg1.time);
	y_average_velocity = (msg0.position.y - msg1.position.y) / (msg0.time - msg1.time);

	// linear model
	x_ = x_average_velocity * (time - msg1.time) + msg1.position.x;
	y_ = y_average_velocity * (time - msg1.time) + msg1.position.y;

	sf::Vector2f network_player_pos(x_, y_);
	return network_player_pos;
}

void LinearPrediction::linearInterpolation(Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& lerp_mode)
{
	sf::Vector2f local_path = predictLinearLocalPath(clock, offset);
	sf::Vector2f network_path = predictLinearNetworkPath(clock, offset);
	//lerp path works better with 100ms lag
	sf::Vector2f lerp_position = lerp(local_path, network_path, 0.1);

	// set position
	lerp_mode ? player.setPosition(lerp_position) : player.setPosition(network_path);

	// add lerped to the history of the local posistions
	keepTrackOfLinearLocalPositoins(lerp_position, clock, offset);
}


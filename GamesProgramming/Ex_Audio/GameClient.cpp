#include "GameClient.h"

GameClient::GameClient() {}

GameClient::~GameClient() {}

inline float GameClient::lerp(float start, float end, float time)
{
	return start * (1.0f - time) + time * end;
}

sf::Vector2f GameClient::lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float& time)
{
	sf::Vector2f temp;
	temp.x = lerp(start.x, end.x, time);
	temp.y = lerp(start.y, end.y, time);
	return temp;
}

sf::Int32 GameClient::getCurrentTime(const sf::Clock& clock, const sf::Int32& offset)
{
	sf::Int32 current_time = clock.getElapsedTime().asMilliseconds();
	return sf::Int32(current_time + (offset));
}

void GameClient::addMessage(PlayerMessage& player_message_send, const Player& player, const sf::Clock& clock, const sf::Int32& offset)
{
	//PlayerMessage player_message_send;
	player_message_send.id = 0;
	player_message_send.position.x = player.getPosition().x;
	player_message_send.position.y = player.getPosition().y;

	//player_message_send.time = c_s;
	player_message_send.time = (float)getCurrentTime(clock, offset);
}

void GameClient::keepTrackOfLinearLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset)
{
	// local message
	PlayerMessage local_message;
	local_message.position.x = player.getPosition().x;
	local_message.position.y = player.getPosition().y;
	local_message.time = (float)getCurrentTime(clock, offset);

	if (linear_local_positions.size() >= linear_message_number) linear_local_positions.pop();
		linear_local_positions.push(local_message);
}

void GameClient::keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset)
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

void GameClient::keepTrackOfLinearNetworkPositions(const PlayerMessage& player_message_receive)
{
	if (linear_network_positions.size() >= linear_message_number) linear_network_positions.pop();
		linear_network_positions.push(player_message_receive);
}

sf::Vector2f GameClient::predictLinearLocalPath(const sf::Clock& clock, const sf::Int32& offset)
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

sf::Vector2f GameClient::predictLinearNetworkPath(const sf::Clock& clock, const sf::Int32& offset)
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

void GameClient::linearInterpolation(Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& lerp_mode)
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

void GameClient::keepTrackOfQuadraticLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset)
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

void GameClient::keepTrackOfQuadraticLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset)
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

void GameClient::keepTrackOfQuadraticNetworkPositions(const PlayerMessage& player_message_receive)
{
	if (quadratic_network_positions.size() >= quadratic_message_number) quadratic_network_positions.pop_back();
		quadratic_network_positions.push_front(player_message_receive);
}

sf::Vector2f GameClient::predictQuadraticLocalPath(const sf::Clock& clock, const sf::Int32& offset)
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

	a_x = (x_average_velocity_2 - x_average_velocity_1);
	a_y = (y_average_velocity_2 - y_average_velocity_1);

	// s = s0 + v0t + ½at2
	x_ = msg2.position.x + (x_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_x) * powf((time - msg2.time), 2));
	y_ = msg2.position.y + (y_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_y) * powf((time - msg2.time), 2));

	sf::Vector2f local_player_pos(x_, y_);
	return local_player_pos;
}

sf::Vector2f GameClient::predictQuadraticNetworkPath(const sf::Clock& clock, const sf::Int32& offset)
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

	a_x = (x_average_velocity_2 - x_average_velocity_1);
	a_y = (y_average_velocity_2 - y_average_velocity_1);

	// s = s0 + v0t + ½at2
	x_ = msg2.position.x + (x_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_x) * powf((time - msg2.time), 2));
	y_ = msg2.position.y + (y_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_y) * powf((time - msg2.time), 2));

	sf::Vector2f network_player_pos(x_, y_);
	return network_player_pos;
}

void GameClient::quadraticInterpolation(Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& lerp_mode)
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

// RECEIVE (from the server's perspective what client is sending for the server to receive) 
// packet layout MUST MATCH the GameServer's receivePacket function packet layout
sf::Packet GameClient::groupIntoPacket(const PlayerMessage& player_message_send)
{
	// message
	sf::Packet packet_to_send;
	packet_to_send << player_message_send;
	return packet_to_send;
}

// SEND (from the client's perspective what server is sending for the client to receive)
// packet layout MUST MATCH the GameServer's groupIntoPacket function packet layout
void GameClient::receivePacket(sf::Packet& packet_receive)
{
	// Extract the variables contained in the packet
	// Packets must match to what the server is sending (e.g.: server is sending a string, and an int, client must be expecting a string and an int, ect.)
	PlayerMessage player_message_receive;
	bool lin_pred;
	bool quad_pred;
	bool lerp_mod;
	// Extract packet into local variables
	if (packet_receive >> player_message_receive >> lin_pred >> quad_pred >> lerp_mod)
	{
		// Data extracted successfully...
		//if (debug_mode) displayMessage(player_message_receive);
		// Deal with the messages from the packet
		linear_prediction = lin_pred;
		quadratic_prediction = quad_pred;
		lerp_mode = lerp_mod;

		// Put position into history of network positions
		keepTrackOfLinearNetworkPositions(player_message_receive);
		keepTrackOfQuadraticNetworkPositions(player_message_receive);
	}
}

// Send a message to the server...
void GameClient::sendPacket(const Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& debug_mode)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RECEIVE (from the server's perspective what client is sending for the server to receive) 
	// packet layout MUST MATCH the GameServer's receivePacket function packet layout
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PlayerMessage player_message_send;
	addMessage(player_message_send, player, clock, offset);

	// Group the variables to send into a packet
	sf::Packet send_packet = groupIntoPacket(player_message_send);

	// Send it over the network (socket is a valid sf::TcpSocket)
	switch (socket.send(send_packet, ip_address, port))
	{
	case sf::Socket::Done:
		// send a packet.
		if (debug_mode) std::cout << "CLIENT: Got one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "CLIENT: No more data to receive now\n";
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "CLIENT: receive didn't return Done\n";
		return;
	}


	/// don't need to clear the packet since all the local variables 
	/// cease to exist once the function is over but it's good to 
	/// keep in mind that if the packet is static it should get cleared
	/// after each use
	/// packet_send.clear();
}

// ...wait for the answer
void GameClient::checkForIncomingPackets(const bool& debug_mode)
{
	while (true) 
	{
		// Try to receive the packet from the other end
		sf::Packet packet_receive;
		sf::IpAddress sender;
		unsigned short senderPort;
		switch (socket.receive(packet_receive, sender, senderPort))
		{
		case sf::Socket::Done:
			// Received a packet.
			if (debug_mode) std::cout << "CLIENT: Got one!\n";
			break;

		case sf::Socket::NotReady:
			// No more data to receive (yet).
			if (debug_mode) std::cout << "CLIENT: No more data to receive now\n";
			return;

		default:
			// Something went wrong.
			if (debug_mode) std::cout << "CLIENT: receive didn't return Done\n";
			return;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SEND (from the client's perspective what server is sending for the client to receive)
		// packet layout MUST MATCH the GameServer's groupIntoPacket function packet layout
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		receivePacket(packet_receive);
	}
}
#include "GameClient.h"

GameClient::GameClient() {}

GameClient::~GameClient() {}

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

void GameClient::keepTrackOfLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset)
{
	// local message
	PlayerMessage local_message;
	local_message.position.x = player.getPosition().x;
	local_message.position.y = player.getPosition().y;
	local_message.time = (float)getCurrentTime(clock, offset);
	if (local_positions.size() > num_messages) local_positions.pop_back();
	local_positions.push_front(local_message);
}

void GameClient::keepTrackOfLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset)
{
	// local message
	PlayerMessage local_message;
	local_message.position.x = vec.x;
	local_message.position.y = vec.y;
	local_message.time = (float)getCurrentTime(clock, offset);
	// 
	if (local_positions.size() > num_messages) local_positions.pop_back();
	local_positions.push_front(local_message);
}

void GameClient::keepTrackOfNetworkPositions(const PlayerMessage& player_message_receive)
{
	if (network_positions.size() > num_messages) network_positions.pop_back();
	network_positions.push_front(player_message_receive);
}

sf::Vector2f GameClient::predict_local_path(const sf::Clock& clock, const sf::Int32& offset)
{
	float x_average_velocity, y_average_velocity;
	PlayerMessage msg0 = local_positions.at(0);
	PlayerMessage msg1 = local_positions.at(1);
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

sf::Vector2f GameClient::predict_network_path(const sf::Clock& clock, const sf::Int32& offset)
{
	float x_average_velocity, y_average_velocity;
	PlayerMessage msg0 = network_positions.at(0);
	PlayerMessage msg1 = network_positions.at(1);
	float time = (float)getCurrentTime(clock, offset);

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity = (msg0.position.x - msg1.position.x) / (msg0.time - msg1.time);
	y_average_velocity = (msg0.position.y - msg1.position.y) / (msg0.time - msg1.time);

	//// linear model
	float x_, y_;
	x_ = x_average_velocity * (time - msg1.time) + msg1.position.x;
	y_ = y_average_velocity * (time - msg1.time) + msg1.position.y;

	sf::Vector2f network_player_pos(x_, y_);
	return network_player_pos;
}

////////////////////////////////////////////////////////////
// Send a message to the server...
//
////////////////////////////////////////////////////////////
void GameClient::sendPacket(const Player& player, const sf::Clock& clock, const sf::Int32& offset)
{
	// message
	// RECEIVE (what server receives) - MUST MATCH packet_receive in the GameServerState
	PlayerMessage player_message_send;

	// Group the variables to send into a packet
	sf::Packet packet_send;
	addMessage(player_message_send, player, clock, offset);
	packet_send << player_message_send;
	// Send it over the network (socket is a valid sf::TcpSocket)
	switch (socket.send(packet_send, ip_address, port))
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

////////////////////////////////////////////////////////////
// ...wait for the answer
//
////////////////////////////////////////////////////////////
void GameClient::checkForIncomingPackets()
{
	while (true) {
		// Try to receive the packet from the other end
		// SEND (to the server) MUST MATCH packet_send in the GameServerState
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

		// MESSAGE FROM THE SERVER
		// Extract the variables contained in the packet
		// Packets must match to what the server is sending (e.g.: server is sending string, client must expect string)
		PlayerMessage player_message_receive;
		if (packet_receive >> player_message_receive)
		{
			// Data extracted successfully...
			//if (debug_mode) displayMessage(player_message_receive);
			// Deal with the messages from the packet
			// Put position into history of network positions

			keepTrackOfNetworkPositions(player_message_receive);
		}
	}
}
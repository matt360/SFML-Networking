#include "GameClientNetwork.h"

GameClientNetwork::GameClientNetwork() {}

GameClientNetwork::~GameClientNetwork() {}

void GameClientNetwork::addMessage(Message& message_send, const Sprite& player, const Sprite& enemy, const sf::Clock& clock, const sf::Int32& offset)
{
	//Message player_message_send;
	message_send.id = 0;
	message_send.player_position.x = player.getPosition().x;
	message_send.player_position.y = player.getPosition().y;
	message_send.enemy_position.x = enemy.getPosition().x;
	message_send.enemy_position.y = enemy.getPosition().y;

	message_send.time = (float)getCurrentTime(clock, offset);
}

// RECEIVE (from the server's perspective what client is sending for the server to receive) 
// packet layout MUST MATCH the GameServerNetwork's receivePacket function packet layout
sf::Packet GameClientNetwork::groupIntoPacket(const Message& message_send)
{
	// message
	sf::Packet packet_to_send;
	packet_to_send << message_send;
	return packet_to_send;
}

// SEND (from the client's perspective what server is sending for the client to receive)
// packet layout MUST MATCH the GameServerNetwork's groupIntoPacket function packet layout
void GameClientNetwork::receivePacket(sf::Packet& packet_receive)
{
	// Extract the variables contained in the packet
	// Packets must match to what the server is sending (e.g.: server is sending a string, and an int, client must be expecting a string and an int, ect.)
	Message message_receive;
	bool lin_pred;
	bool quad_pred;
	bool lerp_mod;
	// Extract packet into local variables
	if (packet_receive >> message_receive >> lin_pred >> quad_pred >> lerp_mod)
	{
		// Data extracted successfully...
		//if (debug_mode) displayMessage(player_message_receive);
		// Deal with the messages from the packet
		linear_prediction = lin_pred;
		quadratic_prediction = quad_pred;
		lerp_mode = lerp_mod;

		// Put player's position into history of network positions for linear interpolation
		Message player_message;
		player_message.player_position.x = message_receive.player_position.x;
		player_message.player_position.y = message_receive.player_position.y;
		player_message.time = message_receive.time;

		player_linear_prediction.keepTrackOfLinearNetworkPositions(player_message);
		player_quadratic_prediction.keepTrackOfQuadraticNetworkPositions(player_message);

		// Put enemy's position into history of network positions for linear interpolation
		Message enemy_message;
		enemy_message.enemy_position.x = message_receive.enemy_position.x;
		enemy_message.enemy_position.y = message_receive.enemy_position.y;
		enemy_message.time = message_receive.time;
		enemy_linear_prediction.keepTrackOfLinearNetworkPositions(enemy_message);
		enemy_quadratic_prediction.keepTrackOfQuadraticNetworkPositions(enemy_message);
	}
}

// Send a message to the server...
void GameClientNetwork::sendPacket(const Sprite& player, const Sprite& enemy, const sf::Clock& clock, const sf::Int32& offset, const bool& debug_mode)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RECEIVE (from the server's perspective what client is sending for the server to receive) 
	// packet layout MUST MATCH the GameServerNetwork's receivePacket function packet layout
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Message player_message_send;
	addMessage(player_message_send, player, enemy, clock, offset);

	// Group the variables to send into a packet
	sf::Packet send_packet = groupIntoPacket(player_message_send);

	// Send it over the network (socket is a valid sf::TcpSocket)
	switch (socket.send(send_packet, Network::ip_address, Network::port))
	{
	case sf::Socket::Partial:
		while (sf::Socket::Done) { socket.send(send_packet, Network::ip_address, Network::port); }
		break;

	case sf::Socket::Done:
		// send a packet.
		if (debug_mode) std::cout << "CLIENT: Got one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "CLIENT: No more data to receive now\n";
		return;

	case sf::Socket::Disconnected:
		established_connection = false;
		return;

	case sf::Socket::Error:
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
void GameClientNetwork::checkForIncomingPackets(const bool& debug_mode)
{
	while (true) 
	{
		// TODO lost ip address and port need to pass it here
		// Try to receive the packet from the other end
		sf::Packet packet_receive;
		switch (socket.receive(packet_receive, Network::ip_address, Network::port))
		{
		case sf::Socket::Partial:
			break;

		case sf::Socket::Done:
			// Received a packet.
			if (debug_mode) std::cout << "CLIENT: Got one!\n";
			break;

		case sf::Socket::NotReady:
			// No more data to receive (yet).
			if (debug_mode) std::cout << "CLIENT: No more data to receive now\n";
			return;

		case sf::Socket::Disconnected:
			established_connection = false;
			return;

		case sf::Socket::Error:
			return;

		default:
			// Something went wrong.
			if (debug_mode) std::cout << "CLIENT: receive didn't return Done\n";
			return;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SEND (from the client's perspective what server is sending for the client to receive)
		// packet layout MUST MATCH the GameServerNetwork's groupIntoPacket function packet layout
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		receivePacket(packet_receive);
	}
}
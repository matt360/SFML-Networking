#include "GameServer.h"

GameServer::GameServer() 
{
	lerp_mode = true;
	linear_prediction = true;
	quadratic_prediction = false;
}

GameServer::~GameServer() {}

void GameServer::addMessage(PlayerMessage& player_message_send, const Player& player, const sf::Clock& clock)
{
	//PlayerMessage player_message_send;
	player_message_send.id = 0;
	player_message_send.position.x = player.getPosition().x;
	player_message_send.position.y = player.getPosition().y;

	player_message_send.time = clock.getElapsedTime().asMilliseconds();
}

// RECEIVE (from the client)
void GameServer::receivePacket(sf::Packet& packet_receive)
{
	// Extract the variables contained in the packet
	PlayerMessage player_message_receive;

	if (packet_receive >> player_message_receive)
	{
		// Data extracted successfully...
		// Deal with the messages from the packet
	}
}

// SEND (to the client)
sf::Packet GameServer::groupIntoPacket(const PlayerMessage& player_message_send)
{
	sf::Packet packet_to_send;
	packet_to_send << player_message_send << linear_prediction << quadratic_prediction << lerp_mode;
	return packet_to_send;
}

// Wait for a message, send an answer.
void GameServer::runUdpServer(const Player& player, const sf::Clock& clock, const bool& debug_mode)
{
	// Wait for a message
	// Receive the packet at the other end
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RECEIVE (from the client) variable layout MUST MATCH the GameClient's groupIntoPacket function's layout //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	receivePacket(packet_receive);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// SEND (to the client) variable layout MUST MATCH the GameClient's receivePacket function's layout //
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Message to send
	PlayerMessage player_message_send;
	addMessage(player_message_send, player, clock);

	// Group the variables to send into a packet
	sf::Packet packet_send = groupIntoPacket(player_message_send);

	// Send it over the network
	switch (socket.send(packet_send, sender, senderPort))
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

	// DEBUG purposes - display sent message
	// Extract the variables contained in the packet
	//if (debug_mode)
	//{
	//	PlayerMessage player_message_send_d;
	//	if (packet_send >> player_message_send_d)
	//	{
	//		// Data extracted successfully...
	//		//displayMessage(player_message_send_d);
	//	}
	//}
}



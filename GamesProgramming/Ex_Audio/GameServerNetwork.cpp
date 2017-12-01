#include "GameServerNetwork.h"

GameServerNetwork::GameServerNetwork() 
{
	lerp_mode = true;
	linear_prediction = true;
	quadratic_prediction = false;
}

GameServerNetwork::~GameServerNetwork() {}

void GameServerNetwork::addMessage(Message& message_send, const Sprite& player, const Sprite& enemy, const sf::Clock& clock)
{
	//Message player_message_send;
	message_send.id = 0;
	message_send.player_position.x = player.getPosition().x;
	message_send.player_position.y = player.getPosition().y;
	message_send.enemy_position.x = enemy.getPosition().x;
	message_send.enemy_position.y = enemy.getPosition().y;

	message_send.time = clock.getElapsedTime().asMilliseconds();
}

// RECEIVE (from the client)
// packet layout MUST MATCH the GameClientNetwork's groupIntoPacket function packet layout
void GameServerNetwork::receivePacket(sf::Packet& packet_receive)
{
	// Extract the variables contained in the packet
	if (packet_receive >> established_connection)
	{
		// Data extracted successfully...
		// Deal with the messages from the packet
	}
}

// SEND (to the client)
// packet layout MUST MATCH the GameClientNetwork's receivePacket function packet layout
sf::Packet GameServerNetwork::groupIntoPacket(const Message& message_send)
{
	sf::Packet packet_to_send;
	packet_to_send << message_send << linear_prediction << quadratic_prediction << lerp_mode;
	return packet_to_send;
}

// Wait for a message, send an answer.
void GameServerNetwork::runUdpServer(const Sprite& player, const Sprite& enemy, const sf::Clock& clock, const bool& debug_mode)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// SEND (to the client) 
	// packet layout MUST MATCH the GameClientNetwork's receivePacket function packet layout
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Message to send
	Message message_send;
	addMessage(message_send, player, enemy, clock);

	// Group the variables to send into a packet
	sf::Packet send_packet = groupIntoPacket(message_send);

	// Send it over the network
	//switch (socket.send(packet_send, Network::ip_address, Network::port))
	switch (socket.send(send_packet, Network::ip_address, Network::port))
	{
	case sf::Socket::Partial:
		// 
		/*
		https://www.sfml-dev.org/tutorials/2.4/network-socket.php
		if only a part of the data was sent in the call, the return status will be sf::Socket::Partial to indicate a partial send.
		If sf::Socket::Partial is returned, you must make sure to handle the partial send properly or else data corruption will occur.
		When sending raw data, you must reattempt sending the raw data at the byte offset where the previous send call stopped.
		*/
		while (socket.send(send_packet, Network::ip_address, Network::port) != sf::Socket::Done) {}
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
		if (debug_mode) std::cout << "CLIENT: Disconnected\n";
		return;

	case sf::Socket::Error:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "CLIENT: receive didn't return Done\n";
		return;
	}
}



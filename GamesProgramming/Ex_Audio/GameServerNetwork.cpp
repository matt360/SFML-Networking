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
	sf::Packet packet_send = groupIntoPacket(message_send);

	// Send it over the network
	//switch (socket.send(packet_send, Network::ip_address, Network::port))
	switch (socket.send(packet_send, Network::ip_address, Network::port))
	{
	case sf::Socket::Partial:
		while (socket.send(packet_send, Network::ip_address, Network::port) != sf::Socket::Done)
		{
		}
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

	// Make the selector wait for data on any socket
	//if (selector.wait())
	//{
	//	if (selector.isReady(socket))
	//	{
	//		sf::UdpSocket* client = new sf::UdpSocket;
	//		// Add the new client to the clients list
	//		clients.push_back(client);

	//		// Add the new client to the selector so that we will
	//		// be notified when he sends something
	//		selector.add(*client);
	//	}
	//	else
	//	{
	//		// The listener socket is not ready, test all other sockets (the clients)
	//		for (auto it = clients.begin(); it != clients.end(); ++it)
	//		{
	//			sf::UdpSocket& client = **it;
	//			if (selector.isReady(client))
	//			{
	//				//////////////////////////////////////////////////////////////////////////////////////////////////////
	//				// SEND (to the client) 
	//				// packet layout MUST MATCH the GameClientNetwork's receivePacket function packet layout
	//				//////////////////////////////////////////////////////////////////////////////////////////////////////
	//				// Message to send
	//				Message player_message_send;
	//				addMessage(player_message_send, player, enemy, clock);

	//				// Group the variables to send into a packet
	//				sf::Packet packet_send = groupIntoPacket(player_message_send);

	//				// Send it over the network
	//				//switch (socket.send(packet_send, Network::ip_address, Network::port))
	//				switch (client.send(packet_send, Network::ip_address, Network::port))
	//				{
	//				case sf::Socket::Partial:
	//					while (sf::Socket::Done) { socket.send(packet_send, Network::ip_address, Network::port); }
	//					break;

	//				case sf::Socket::Done:
	//					// Received a packet.
	//					if (debug_mode) std::cout << "CLIENT: Got one!\n";
	//					break;

	//				case sf::Socket::NotReady:
	//					// No more data to receive (yet).
	//					if (debug_mode) std::cout << "CLIENT: No more data to receive now\n";
	//					return;

	//				case sf::Socket::Disconnected:
	//					established_connection = false;
	//					return;

	//				case sf::Socket::Error:
	//					return;

	//				default:
	//					// Something went wrong.
	//					if (debug_mode) std::cout << "CLIENT: receive didn't return Done\n";
	//					return;
	//				}
	//			}
	//		}
	//	}
	//}
}



/*
void GameServerNetwork::runUdpServer(const Sprite& player, const sf::Clock& clock, const bool& debug_mode)
{
	/// Wait for a message
	Receive the packet at the other end
		TODO shouldn't be receiving anymore
		TODO thats where its loosing it
		sf::Packet packet_receive;
	switch (socket.receive(packet_receive, Network::ip_address, Network::port))
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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// RECEIVE (from the client) 
	//// packet layout MUST MATCH the GameClientNetwork's groupIntoPacket function packet layout
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	receivePacket(packet_receive);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// SEND (to the client) 
	// packet layout MUST MATCH the GameClientNetwork's receivePacket function packet layout
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Message to send
	Message player_message_send;
	addMessage(player_message_send, player, clock);

	// Group the variables to send into a packet
	sf::Packet packet_send = groupIntoPacket(player_message_send);

	// Send it over the network
	switch (socket.send(packet_send, Network::ip_address, Network::port))
	{
	case sf::Socket::Partial:
		while (sf::Socket::Done) { socket.send(packet_send, Network::ip_address, Network::port); }
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

	// DEBUG purposes - display sent message
	// Extract the variables contained in the packet
	//if (debug_mode)
	//{
	//	Message player_message_send_d;
	//	if (packet_send >> player_message_send_d)
	//	{
	//		// Data extracted successfully...
	//		//displayMessage(player_message_send_d);
	//	}
	//}
}
*/



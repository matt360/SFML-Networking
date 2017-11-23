#include "NetworkServer.h"

NetworkServer::NetworkServer()
{
	bool established_connection = false;
}


NetworkServer::~NetworkServer()
{
}

void NetworkServer::establishConnectionWithClient(const bool& debug_mode)
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
		if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
		return;
	}

	// Extract the variables contained in the packet
	// RECEIVE (from the client) MUST MATCH packet_send in the GameClientState
	bool hello;
	if (packet_receive >> hello)
	{
		// Deal with the messages from the packet
		// The message from the client
		established_connection = hello;
		//if (debug_message) displayReceiveMessage(hello);
	}

	//////////////////////////////////////////////////////////////////////
	// SEND (to the client) MUST MATCH packet_receive in the GameClientState //
	//////////////////////////////////////////////////////////////////////
	sf::Packet packet_send;
	// Message to send
	// Group the variables to send into a packet
	//established_connection = true;
	sf::Int32 server_time = clock.getElapsedTime().asMilliseconds();
	packet_send << server_time << established_connection;
	//sf::sleep(sf::milliseconds(1000));
	// Send it over the network
	switch (socket.send(packet_send, sender, senderPort))
	{
	case sf::Socket::Done:
		// Received a packet.
		if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";

		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
		return;
	}

	// DEBUG purposes - since we are sending server time it's not needed
	// Extract the variables contained in the packet
	//if (debug_message)
	//{
	//	if (packet_send >> server_time >> established_connection)
	//	{
	//		// Data extracted successfully...
	//		if (debug_message) displaySendMessage(server_time);
	//	}
	//}
}

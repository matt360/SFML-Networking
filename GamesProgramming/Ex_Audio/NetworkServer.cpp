#include "NetworkServer.h"

NetworkServer::NetworkServer() {}


NetworkServer::~NetworkServer() {}

// MATCH 1 (variables must match witch MATCH 1 in the NetworkClient)
sf::Packet NetworkServer::groupIntoPacket()
{
	// Message to send
	sf::Packet packet_to_send;
	// Group the variables to send into a packet
	sf::Int32 server_time = clock.getElapsedTime().asMilliseconds();
	packet_to_send << server_time << established_connection;
	// TODO desiplay and let adjust this
	sf::sleep(sf::milliseconds(100));

	return packet_to_send;
}

// MATCH 2 (variables must match witch MATCH 2 in the NetworkClient)
void NetworkServer::receivePacket(sf::Packet& packet_receive)
{
	// Extract the variables contained in the packet
	// Deal with the messages from the packet
	bool hello;
	if (packet_receive >> hello)
	{
		// The message from the client
		established_connection = hello;
		//if (debug_message) displayReceiveMessage(hello);
	}
}

// Wait for a message
void NetworkServer::establishConnectionWithClient(const bool& debug_mode)
{
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

	///////////////////////////////////////////////////////////////////////////
	// RECEIVE (from the client) MUST MATCH packet_send in the NetworkClient //
	///////////////////////////////////////////////////////////////////////////
	receivePacket(packet_receive);

	/////////////////////////////////////////////////////////////////////////
	// SEND (to the client) MUST MATCH packet_receive in the NetworkClient //
	/////////////////////////////////////////////////////////////////////////
	sf::Packet send_packet = groupIntoPacket();

	// Send it over the network
	switch (socket.send(send_packet, sender, senderPort))
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

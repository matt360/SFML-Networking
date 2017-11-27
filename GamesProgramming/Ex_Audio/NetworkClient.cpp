#include "NetworkClient.h"

NetworkClient::NetworkClient() {}

NetworkClient::~NetworkClient() {}

// MATCH 1 (variables must match witch MATCH 1 in the NetworkServer)
void NetworkClient::receivePacket(sf::Packet& packet_receive)
{
	// Extract the variables contained in the packet
	sf::Int32 server_time;
	// Packets must match to what the server is sending (e.g.: server is sending string, client must expect string)
	if (packet_receive >> server_time >> established_connection)
	{
		// Data extracted successfully...
		// Deal with the messages from the packet
		//if (debug_message) displayReceiveMessage(server_time);

		// end timing latency
		end_timing_latency = clock.getElapsedTime().asMilliseconds();
		std::cout << "end_timing_latency: " << end_timing_latency << "\n";
		latency = (end_timing_latency - start_timing_latency);
		std::cout << "latency: " << latency << "\n";
		// calculate server time
		sf::Int32 client_time = clock.getElapsedTime().asMilliseconds();
		std::cout << "client_time: " << client_time << "\n";
		// server_time from the message
		offset = ((server_time + (0.5 * latency)) - client_time);
		std::cout << "offset: " << offset << "\n";
	}
}

// MATCH 2 (variables must match witch MATCH 2 in the NetworkServer)
sf::Packet NetworkClient::groupIntoPacket()
{
	// Group the variables to send into a packet
	sf::Packet packet_to_send;
	// Message to send
	bool hello = true;
	packet_to_send << hello;

	return packet_to_send;
}

// Send a message to the server...
void NetworkClient::sendPacketToServer(const bool& debug_mode)
{
	/////////////////////////////////////////////////////////////////////////////////////
	// RECEIVE (what server receives) - MUST MATCH packet_receive in the NetworkServer //
	/////////////////////////////////////////////////////////////////////////////////////
	sf::Packet packet_send = groupIntoPacket();

	// Send it over the network
	switch (socket.send(packet_send, ip_address, port))
	{
	case sf::Socket::Done:
		// send a packet.
		if (debug_mode) std::cout << "\nCLIENT: Sent one!\n";
		/*send_packet = false;
		std::cout << "send_packet is false" << "\n";*/
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: Can't send now\n";
		send_packet = true;
		std::cout << "send_packet is true" << "\n";
		//if (debug_mode) 
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: send didn't return Done\n";
		return;
	}

	// DEBUG purposes
	// Extract the variables contained in the packet
	//bool hello;
	//if (packet_send >> hello)
	//{
	//	// Data extracted successfully...
	//	//if (debug_message) displaySendMessage(hello);
	//	send_packet = false;
	//	std::cout << "send_packet is false" << "\n";
	//}

	/// don't need to clear the packet since all the local variables 
	/// cease to exist once the function is over but it's good to 
	/// keep in mind that if the packet is static it should get cleared
	/// after each use
	/// packet_send.clear();
}

// ...wait for the answer
void NetworkClient::checkForIncomingPacketsFromServer(const bool& debug_mode)
{
	// CHECK FOR INCOMING PACKETS
	while (true)
	{
		// Try to receive the packet from the other end
		//////////////////////////////////////////////////////////////////////
		// SEND (to the server) MUST MATCH packet_send in the NetworkServer //
		//////////////////////////////////////////////////////////////////////
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

		receivePacket(packet_receive);
	}
}

void NetworkClient::establishConnectionWithServer(const bool& debug_mode)
{
	// send message to the server...
	if (send_packet)
	{
		// start timing latency
		start_timing_latency = clock.getElapsedTime().asMilliseconds();
		std::cout << "start_timing_latency: " << start_timing_latency << "\n";
		sendPacketToServer(debug_mode);
	}

	// ...wait for the answer
	checkForIncomingPacketsFromServer(debug_mode);
}

// Send a message to the server...
void NetworkClient::sendPacketToServer()
{
	/////////////////////////////////////////////////////////////////////////////////////
	// RECEIVE (what server receives) - MUST MATCH packet_receive in the NetworkServer //
	/////////////////////////////////////////////////////////////////////////////////////
	// Group the variables to send into a packet
	sf::Packet packet_send;
	// Message to send
	bool hello = true;
	packet_send << hello;

	// Send it over the network
	switch (socket.send(packet_send, ip_address, port))
	{
	case sf::Socket::Done:
		// send a packet.
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		return;

	default:
		// Something went wrong.
		return;
	}

	// DEBUG purposes
	// Extract the variables contained in the packet
	//bool hello;
	//if (packet_send >> hello)
	//{
	//	// Data extracted successfully...
	//	//if (debug_message) displaySendMessage(hello);
	//	set_ready = false;
	//	std::cout << "send_packet is false" << "\n";
	//}
	/// don't need to clear the packet since all the local variables 
	/// cease to exist once the function is over but it's good to 
	/// keep in mind that if the packet is static it should get cleared
	/// after each use
	/// packet_send.clear();
}

// ...wait for the answer
void NetworkClient::checkForIncomingPacketsFromServer()
{
	// CHECK FOR INCOMING PACKETS
	while (true)
	{
		// Try to receive the packet from the other end
		//////////////////////////////////////////////////////////////////////
		// SEND (to the server) MUST MATCH packet_send in the NetworkServer //
		//////////////////////////////////////////////////////////////////////
		sf::Packet packet_receive;
		sf::IpAddress sender;
		unsigned short senderPort;
		switch (socket.receive(packet_receive, sender, senderPort))
		{
		case sf::Socket::Done:
			// Received a packet.
			break;

		case sf::Socket::NotReady:
			// No more data to receive (yet).
			return;

		default:
			// Something went wrong.
			return;
		}

		// Extract the variables contained in the packet
		// Packets must match to what the server is sending (e.g.: server is sending string, client must expect string)
		bool set_ready;
		if (packet_receive >> set_ready)
		{
			// Data extracted successfully...
			// Deal with the messages from the packet
			ready = set_ready;
		}
	}
}

void NetworkClient::establishConnectionWithServer()
{
	// send message to the server...
	sendPacketToServer();

	// ...wait for the answer
	checkForIncomingPacketsFromServer();
}

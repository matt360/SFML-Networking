#include "NetworkClient.h"
#include <iostream>

NetworkClient::NetworkClient(sf::RenderWindow* hwnd,
	Input* in,
	GameState* st,
	NetworkState* net_st,
	sf::UdpSocket* udp_socket,
	sf::IpAddress* ip,
	unsigned short* port_number,
	sf::Clock* cl,
	sf::Int32* of)
{
	window = hwnd;
	input = in;
	state = st;
	network_state = net_st;
	socket = udp_socket;
	ip_address = ip;
	port = port_number;
	clock = cl;
	offset = of;

	send_packet = true;
	established_connection = false;
	//client_time = 0;
	//server_offset = 0;

	ready = false;
	server = false;
	client = false;
	debug_mode = false;
	debug_message = false;

	// Network text
	font.loadFromFile("font/advanced_pixel-7.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Press 'S' to be a server\n\nPress 'C' to be a client\n\nPress Enter to Play");
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::handleInput()
{
	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		// player ready to play
		ready = true;
	}

	// toggle debug mode to display socket messages
	if (input->isKeyDown(sf::Keyboard::D))
	{
		input->setKeyUp(sf::Keyboard::D);
		debug_mode = !debug_mode;
	}
	// toggle debug messages to display messages
	if (input->isKeyDown(sf::Keyboard::M))
	{
		input->setKeyUp(sf::Keyboard::M);
		debug_message = !debug_message;
	}
}

// error handling before connecting
// error while the game is running
// states
// what I want to send
// anything that happens to the client shouldn't affect the server player
// anything that happens to the server the client must handle accordingly - if the server is dead - try to reconnect for a ceratin time - take to the network state

void NetworkClient::displayReceiveMessage(sf::Int32 time)
{
	// Message FROM the server
	std::cout << "\n\nCLIENT: Message received from the server:";
	// Data extracted successfully...
	std::cout << "\nCLIENT: server's time: " << time;
}

void NetworkClient::displaySendMessage(sf::Int32 time)
{
	// Message sent TO the server
	std::cout << "\n\nCLIENT: Message sent to the server:";
	// Data extracted successfully...
	std::cout << "\nCLIENT: client's time: " << time;
}

void NetworkClient::displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port)
{
	// Message FROM the server
	std::cout << "\nCLIENT: IP: " << sender;
	std::cout << "\nCLIENT: port: " << sender_port;
	std::cout << "\nCLIENT: time: " << time;
}

sf::Int32 NetworkClient::getCurrentTime()
{
	sf::Int32 current_time = clock->getElapsedTime().asMilliseconds();
	return sf::Int32(current_time + (*offset));
}

////////////////////////////////////////////////////////////
// Send a message to the server...
//
////////////////////////////////////////////////////////////
void NetworkClient::sendPacketToServer()
{
	// message
	// RECEIVE (what server receives) - MUST MATCH packet_receive in the GameServer
	// Group the variables to send into a packet
	sf::Packet packet_send;
	// Message to send
	float hello = 1.0f;
	packet_send << hello;

	// Send it over the network (socket is a valid sf::TcpSocket)
	switch (socket->send(packet_send, *ip_address, *port))
	{
	case sf::Socket::Done:
		// send a packet.
		if (debug_mode) std::cout << "\nCLIENT: Sent one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: Can't send now\n";
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: send didn't return Done\n";
		return;
	}

	// DEBUG purposes
	// Extract the variables contained in the packet
	if (packet_send >> hello)
	{
			// Data extracted successfully...
		if (debug_message) displaySendMessage(hello);
		send_packet = false;
		std::cout << "send_packet is false" << "\n";
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
void NetworkClient::checkForIncomingPacketsFromServer()
{
	////////////////////////////////////////////////////////////////////
	// CHECK FOR INCOMING PACKETS                                     //
	////////////////////////////////////////////////////////////////////
	while (true) {
		// Try to receive the packet from the other end
		// SEND (to the server) MUST MATCH packet_send in the GameServer
		sf::Packet packet_receive;
		sf::IpAddress sender;
		unsigned short senderPort;
		switch (socket->receive(packet_receive, sender, senderPort))
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

		sf::Int32 server_time;
		// Extract the variables contained in the packet
		// Packets must match to what the server is sending (e.g.: server is sending string, client must expect string)
		if (packet_receive >> server_time >> established_connection)
		{
			// Data extracted successfully...
			// Deal with the messages from the packet
			if (debug_message) displayReceiveMessage(server_time);

			// end timing latency
			end_timing_latency = clock->getElapsedTime().asMilliseconds();
			std::cout << "end_timing_latency: " << end_timing_latency << "\n";
			latency = (end_timing_latency - start_timing_latency);
			std::cout << "latency: " << latency << "\n";
			// calculate server time
			sf::Int32 client_time = clock->getElapsedTime().asMilliseconds();
			std::cout << "client_time: " << client_time << "\n";
			// server_time from the message
			(*offset) = server_time + 0.5 * latency - client_time;
			std::cout << "offset: " << *offset << "\n";
		}
	}
}

void NetworkClient::establishConnectionWithServer()
{
	// send message to the server...
	if (send_packet) 
	{
		// start timing latency
		start_timing_latency = clock->getElapsedTime().asMilliseconds();
		std::cout << "start_timing_latency: " << start_timing_latency << "\n";
		sendPacketToServer();
	}

	// ...wait for the answer
	checkForIncomingPacketsFromServer();
}

void NetworkClient::update()
{
	text.setString("\n\nYou're the client\n\nConnecting to the server...\n\nPress Enter to Play");

	if (!established_connection)
	{
		establishConnectionWithServer();
		std::cout << "function call: establishConnectionWithServer()\n";
		std::cout << "function call: getCurrentTime(): " << getCurrentTime() << "\n";
	}

	std::cout << "function call: getCurrentTime(): " << getCurrentTime() << "\n";

	if (ready && established_connection)
	{
		// extra house keeping
		/*if (*network_state == NetworkState::CLIENT)
		{*/
			// TODO probably want to do something better than this
			*state = GameState::GAME_CLIENT;
		/*}
		else 
		{
			ready = false;
		}*/
	}
}

void NetworkClient::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

//void NetworkClient::beginDraw()
//{
//	window->clear(sf::Color(0, 0, 0));
//}
//
//void NetworkClient::endDraw()
//{
//	window->display();
//}
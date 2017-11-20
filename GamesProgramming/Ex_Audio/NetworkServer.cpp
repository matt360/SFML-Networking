#include "NetworkServer.h"
#include <iostream>

NetworkServer::NetworkServer(sf::RenderWindow* hwnd, Input* in, GameState* st, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short* port_number, sf::Clock* cl, float* cur_t)
{
	window = hwnd;
	input = in;
	socket = udp_socket;
	ip_address = ip;
	port = port_number;
	state = st;
	clock = cl;
	current_time = cur_t;

	readyToPlay = false;
	server = false;
	client = false;
	debug_mode = true;
	debug_message = true;
	network_state = NetworkState::NONE;

	// Network text
	font.loadFromFile("font/advanced_pixel-7.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Press 'S' to be a server\n\nPress 'C' to be a client\n\nPress Enter to Play");
}

NetworkServer::~NetworkServer()
{
}

void NetworkServer::handleInput()
{
	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		// player ready to play
		readyToPlay = true;
	}

	// Client or server ?
	// toggle being the server
	if (input->isKeyDown(sf::Keyboard::S))
	{
		input->setKeyUp(sf::Keyboard::S);
		// set readiness for being the server
		server = true;
		// setting client to false let's us change the decision and to become the cleint
		client = false;
		network_state = NetworkState::SERVER;
	}
	// toggle being the client
	if (input->isKeyDown(sf::Keyboard::C))
	{
		input->setKeyUp(sf::Keyboard::C);
		// set readiness for being the client
		client = true;
		// setting server to false let's us change the decision and become the server
		server = false;
		network_state = NetworkState::CLIENT;
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

void NetworkServer::displayMessage(sf::Int32 time)
{
	// The message from the client
	std::cout << "\n\nSERVER: Message received from the client:";
	// Data extracted successfully...
	std::cout << "\nSERVER: client's time: " << time;
}

void NetworkServer::displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port)
{
	// The message from the client
	std::cout << "\nSERVER: client's IP: " << sender;
	std::cout << "\nSERVER: client's port: " << sender_port;
	std::cout << "\nSERVER: client' time: " << time;
}

void NetworkServer::establishConnectionWithClient()
{
	// Wait for a message
	// Receive the packet at the other end
	sf::Packet packet_receive;
	sf::IpAddress sender;
	unsigned short senderPort;
	switch (socket->receive(packet_receive, sender, senderPort))
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

	// Extract the variables contained in the packet
	// RECEIVE (from the client) MUST MATCH packet_send in the GameClient
	float receive_time;
	if (packet_receive >> receive_time)
	{
		// The message from the client
		if (debug_message) displayMessage(receive_time, sender, senderPort);
		server_receive_time = receive_time;
	}

	//////////////////////////////////////////////////////////////////////
	// SEND (to the client) MUST MATCH packet_receive in the GameClient //
	//////////////////////////////////////////////////////////////////////
	sf::Packet packet_send;
	// Message to send
	sf::Int32 send_time = clock->getElapsedTime().asMilliseconds();
	//addMessage(player_message_send);
	// Group the variables to send into a packet
	packet_send << send_time;
	// Send it over the network
	switch (socket->send(packet_send, sender, senderPort))
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

	// DEBUG purposes
	// Extract the variables contained in the packet
	if (debug_message)
	{
		if (packet_send >> send_time)
		{
			// Data extracted successfully...
			if (debug_message) displayMessage(send_time);
			server_send_time = send_time;
		}
	}
}

void NetworkServer::update()
{
	// Client or server ?
	if (server)
	{
		// create server socket
		//createServerSocket();
		text.setString("Connecting...\n\nYou're the server\n\nPress Enter to Play");

		server = false;
	}
	if (client)
	{
		// create client socket
		//createClientSocket();
		// message - joined the server
		text.setString("Connecting...\n\nYou're the client\n\nPress Enter to Play");

		client = false;
	}

	switch (network_state)
	{
	case (NetworkState::SERVER):
		establishConnectionWithClient();
		break;
	case (NetworkState::CLIENT):
		//establishConnectionWithServer();
		break;
		/*default:

		break;*/
	}

	if (readyToPlay)
	{
		switch (network_state)
		{
		case (NetworkState::SERVER):
			*state = GameState::GAME_SERVER;
			break;
		case (NetworkState::CLIENT):
			*state = GameState::GAME_CLIENT;
			break;
		default:
			readyToPlay = false;
			break;
		}
	}
}

void NetworkServer::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

void NetworkServer::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void NetworkServer::endDraw()
{
	window->display();
}

//void NetworkServer::do_once()
//{
//	std::call_once(ask_flag, [&]() {
//		std::cout << "Do you want to be a server (s) or a client (c)? ";
//		//std::cin >> who; 
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
//			networkState = NetworkState::SERVER;
//		}
//		else {
//			networkState = NetworkState::CLIENT;
//		}
//	}
//	);
//}
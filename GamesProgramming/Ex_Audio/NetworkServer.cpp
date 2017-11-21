#include "NetworkServer.h"
#include <iostream>

NetworkServer::NetworkServer(sf::RenderWindow* hwnd,
	Input* in,
	GameState* st,
	NetworkState* net_st,
	sf::UdpSocket* udp_socket,
	sf::IpAddress* ip,
	unsigned short* port_number,
	sf::Clock* cl,
	sf::Int32* cur_t)
{
	window = hwnd;
	input = in;
	state = st;
	network_state = net_st;
	socket = udp_socket;
	ip_address = ip;
	port = port_number;
	clock = cl;
	current_time = cur_t;

	established_connection = false;
	client_time = 0;
	server_time = 0;

	readyToPlay = false;
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

void NetworkServer::displayReceiveMessage(sf::Int32 time)
{
	// The message from the client
	std::cout << "\n\nSERVER: Message received from the client:";
	// Data extracted successfully...
	std::cout << "\nSERVER: client's time: " << time;
}

void NetworkServer::displaySendMessage(sf::Int32 time)
{
	// The message from the client
	std::cout << "\n\nSERVER: Message sent to the client:";
	// Data extracted successfully...
	std::cout << "\nSERVER: server's time: " << time;
}

void NetworkServer::displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port)
{
	// The message from the client
	std::cout << "\nSERVER: IP: " << sender;
	std::cout << "\nSERVER: port: " << sender_port;
	std::cout << "\nSERVER: time: " << time;
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
	// RECEIVE (from the client) MUST MATCH packet_send in the GameClient
	//float receive_time;
	if (packet_receive >> client_time)
	{
		// Deal with the messages from the packet
		//server_receive_time = receive_time;
		// The message from the client
		server_time = clock->getElapsedTime().asMilliseconds();
		*current_time = server_time - client_time;
		if (debug_message) displayReceiveMessage(client_time);
	}

	//////////////////////////////////////////////////////////////////////
	// SEND (to the client) MUST MATCH packet_receive in the GameClient //
	//////////////////////////////////////////////////////////////////////
	if (*current_time != 0)
	{
		sf::Packet packet_send;
		// Message to send
		//server_time = clock->getElapsedTime().asMilliseconds();
		// Group the variables to send into a packet
		established_connection = true;
		packet_send << *current_time << established_connection;
		// Send it over the network
		switch (socket->send(packet_send, sender, senderPort))
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

		// DEBUG purposes
		// Extract the variables contained in the packet
		if (debug_message)
		{
			if (packet_send >> *current_time >> established_connection)
			{
				// Data extracted successfully...
				//server_send_time = send_time;
				if (debug_message) displaySendMessage(server_time);
			}
		}
	}
}

void NetworkServer::update()
{	
	text.setString("\n\nYou're the server\n\nWaiting for the client...\n\nPress Enter to Play");

	establishConnectionWithClient();

	if (readyToPlay)
	{
		// extra house keeping
		if (*network_state == NetworkState::SERVER)
		{
			*state = GameState::GAME_SERVER;
		}
		else
		{
			// TODO probably want to do something better than this
			readyToPlay = false;
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
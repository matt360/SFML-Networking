#include "NetworkServer.h"
#include <iostream>

NetworkServer::NetworkServer(sf::RenderWindow* hwnd,
	Input* in,
	GameState* st,
	NetworkStateEnum* net_st,
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

	established_connection = false;
	/*client_time = 0;
	server_offset = 0;*/

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

NetworkServer::~NetworkServer()
{
}

void NetworkServer::handleInput()
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
	float hello;
	if (packet_receive >> hello)
	{
		// Deal with the messages from the packet
		// The message from the client
		established_connection = (int)hello;
		if (debug_message) displayReceiveMessage(hello);
	}

	//////////////////////////////////////////////////////////////////////
	// SEND (to the client) MUST MATCH packet_receive in the GameClient //
	//////////////////////////////////////////////////////////////////////
	sf::Packet packet_send;
	// Message to send
	//server_offset = clock->getElapsedTime().asMilliseconds();
	// Group the variables to send into a packet
	//established_connection = true;
	sf::Int32 server_time = clock->getElapsedTime().asMilliseconds();
	packet_send << server_time << established_connection;
	std::cout << "server_time: " << server_time << "\n";
	//sf::sleep(sf::milliseconds(1000));
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
		if (packet_send >> server_time >> established_connection)
		{
			// Data extracted successfully...
			if (debug_message) displaySendMessage(server_time);
		}
	}
}

void NetworkServer::update()
{	
	text.setString("\n\nYou're the server\n\nWaiting for the client...\n\nPress Enter to Play");

	establishConnectionWithClient();

	sf::Int32 server_time = clock->getElapsedTime().asMilliseconds();
	std::cout << "server_time: " << server_time << "\n";

	if (ready && established_connection)
	{
		// TODO extra house keeping
		/*if (*network_state == NetworkStateEnum::SERVER)
		{*/
			*state = GameState::GAME_SERVER;
		//}
		//else
		//{
		//	// TODO probably want to do something better than this
		//	readyToPlay = false;
		//}
	}
}

void NetworkServer::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

//void NetworkServer::beginDraw()
//{
//	window->clear(sf::Color(0, 0, 0));
//}
//
//void NetworkServer::endDraw()
//{
//	window->display();
//}
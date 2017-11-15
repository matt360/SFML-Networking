#include "Network.h"
#include <iostream>

Network::Network(sf::RenderWindow* hwnd, Input* in, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short *port_number, GameState* st)
{
	window = hwnd;
	input = in;
	socket = udp_socket;
	ip_address = ip;
	port = port_number;
	state = st;

	readyToPlay = false;
	network_state = NetworkState::NONE;

	// Network text
	font.loadFromFile("font/arial.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Press 'S' to be a server\n\nPress 'C' to be a client\n\nPress Enter to Play");
}

Network::~Network()
{
}

void Network::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		readyToPlay = true;
	}	
}

// error handling before connecting
// error while the game is running
// states
// what I want to send
// anything that happens to the client shouldn't affect the server player
// anything that happens to the server the client must handle accordingly - if the server is dead - try to reconnect for a ceratin time - take to the network state

void Network::serverSocket()
{
	// //////////////////////////////
	// Create a socket to receive a message from anyone
	socket->setBlocking(false);
	// unbinding the socket prevents the socket binding failure if the person tries to bind the port more than once.
	// in this case the server will always use the same port
	socket->unbind();
	// Listen to messages on the specified port
	if (socket->bind(*port) != sf::Socket::Done)
		return;
	std::cout << "Server is listening to port " << *port << ", waiting for a message... " << std::endl;
	/////////////////////////////////
}

void Network::clientSocket()
{
	//////////////////////////////////////////
	sf::IpAddress server(*ip_address);
	socket->setBlocking(false);

	// Ask for the server address
	/*do
	{
	std::cout << "Type the address or name of the server to connect to: ";
	std::cin >> server;
	} while (server == sf::IpAddress::None);*/

	// Create a socket for communicating with the server

	///////////////////////////////////////////
}

void Network::update(float dt)
{
	// Client or server ?
	if (input->isKeyDown(sf::Keyboard::S))
	{
		input->setKeyUp(sf::Keyboard::S);
		// create server socket
		serverSocket();
		text.setString("Connecting...\n\nYou're the server\n\nPress Enter to Play");

		//*state = GameState::GAME_SERVER;
		network_state = NetworkState::SERVER;
	}
	if (input->isKeyDown(sf::Keyboard::C))
	{
		input->setKeyUp(sf::Keyboard::C);
		// create client socket
		clientSocket();
		// message - joined the server
		text.setString("Connecting...\n\nYou're the client\n\nPress Enter to Play");

		//*state = GameState::GAME_CLIENT;
		network_state = NetworkState::CLIENT;
	}

	if (readyToPlay)
	{
		switch (network_state)
		{
		case (NetworkState::CLIENT):
			*state = GameState::GAME_CLIENT;
			readyToPlay = false;
			break;
		case (NetworkState::SERVER):
			*state = GameState::GAME_SERVER;
			readyToPlay = false;
			break;
		default:
			*state = GameState::NETWORK;
			readyToPlay = false;
			break;
		}
	}
}

void Network::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

void Network::beginDraw()
{
	window->clear(sf::Color(100, 149, 237));
}

void Network::endDraw()
{
	window->display();
}







//void Network::do_once()
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
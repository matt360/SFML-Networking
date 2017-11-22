#include "NetworkSelect.h"
#include <iostream>

NetworkSelect::NetworkSelect(sf::RenderWindow* hwnd,
	Input* in,
	GameState* st,
	NetworkState* net_st,
	sf::UdpSocket* udp_socket, 
	sf::IpAddress* ip, 
	unsigned short* port_number)
{
	window = hwnd;
	input = in;
	socket = udp_socket;
	ip_address = ip;
	port = port_number;
	state = st;
	network_state = net_st;

	readyToConnect = false;
	server = false;
	client = false;
	debug_mode = true;
	debug_message = true;

	// NetworkSelect text
	font.loadFromFile("font/advanced_pixel-7.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Press 'S' to be the server\n\nPress 'C' to be the client\n\n");
}

NetworkSelect::~NetworkSelect()
{
}

void NetworkSelect::handleInput()
{
	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		// player ready to play
		readyToConnect = true;
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
		*network_state = NetworkState::SERVER;
	}
	// toggle being the client
	if (input->isKeyDown(sf::Keyboard::C))
	{
		input->setKeyUp(sf::Keyboard::C);
		// set readiness for being the client
		client = true;
		// setting server to false let's us change the decision and become the server
		server = false;
		*network_state = NetworkState::CLIENT;
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

void NetworkSelect::createServerSocket()
{
	//////////////////////////////////////////
	// Create a socket to receive a message from anyone
	socket->setBlocking(false);
	// unbinding the socket prevents the socket binding failure if the person tries to bind the port more than once.
	// in this case the server will always use the same port
	socket->unbind();
	// Listen to messages on the specified port
	if (socket->bind(*port) != sf::Socket::Done) return;

	if (debug_mode) std::cout << "Server is listening to port " << *port << ", waiting for a message...\n";
	//////////////////////////////////////////
}

void NetworkSelect::createClientSocket()
{
	//////////////////////////////////////////
	socket->unbind();
	socket->setBlocking(false);
	
	// Ask for the server address
	/*do
	{
	std::cout << "Type the address or name of the server to connect to: ";
	std::cin >> server;
	} while (server == sf::IpAddress::None);*/
	///////////////////////////////////////////
}

void NetworkSelect::update()
{
	// Client or server ?
	if (server)
	{
		// create server socket
		createServerSocket();
		// display text
		text.setString(
			"You're the Server\n\n"
			"Press Enter to establish the connection with\n\n"
			"The Client\n\n"
			"Press 'S' to be the server\n\n"
			"Press 'C' to be the client\n\n");

		server = false;
	}
	if (client)
	{
		// create client socket
		createClientSocket();
		// display text
		text.setString(
			"You're the Client\n\n"
			"Press Enter to establish the connection with\n\n"
			"The Server\n\n"
			"Press 'S' to be the server\n\n"
			"Press 'C' to be the client\n\n");

		client = false;
	}

	if (readyToConnect) 
	{
		switch (*network_state)
		{
		case (NetworkState::SERVER):
			*state = GameState::NETWORK_SERVER;
			break;
		case (NetworkState::CLIENT):
			*state = GameState::NETWORK_CLIENT;
			break;
		default:
			readyToConnect = false;
			break;
		}
	}
}

void NetworkSelect::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

//void NetworkSelect::beginDraw()
//{
//	window->clear(sf::Color(0, 0, 0));
//}
//
//void NetworkSelect::endDraw()
//{
//	window->display();
//}
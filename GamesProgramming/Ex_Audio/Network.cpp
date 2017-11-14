#include "Network.h"
#include <iostream>

Network::Network(sf::RenderWindow* hwnd, Input* in, sf::UdpSocket* udp_socket, unsigned short *port_number, const std::string* server_address)
{
	window = hwnd;
	input = in;
	port = port_number;
	socket = udp_socket;
	address = server_address;
	ip_address = *server_address;
	state = GameState::NETWORK;
	networkState = NetworkState::NONE;

	// Network text
	font.loadFromFile("font/arial.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Connecting...\n\nPress Enter to Play");
}

Network::~Network()
{
}

GameState Network::getState()
{
	return state;
}

void Network::handleInput(float dt)
{

	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		state = GameState::LEVEL;
	}
	else
	{
		state = GameState::NETWORK;
	}
}

void Network::serverSocket()
{
	// //////////////////////////////
	// Create a socket to receive a message from anyone
	// sf::UdpSocket socket;
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

////////////////////////////////////////////////////////////
/// Launch a server, wait for a message, send an answer.
///
////////////////////////////////////////////////////////////
void Network::runUdpServer()
{
	// Wait for a message
	char in[128];
	std::size_t received;
	sf::IpAddress sender;
	unsigned short senderPort;
	if (socket->receive(in, sizeof(in), received, sender, senderPort) != sf::Socket::Done)
		return;
	std::cout << "Message received from client " << sender << ": \"" << in << "\"" << std::endl;

	// Send an answer to the client
	const char out[] = "Hi, I'm the server";
	if (socket->send(out, sizeof(out), sender, senderPort) != sf::Socket::Done)
		return;
	std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;
}

// error handling before connecting
// error while the game is running
// states
// what I want to send
// anything that happens to the client shouldn't affect the server player
// anything that happens to the server the client must handle accordingly - if the server is dead - try to reconnect for a ceratin time - take to the network state

void Network::clientSocket()
{
	// Ask for the server address
	//////////////////////////////////////////
	sf::IpAddress server(*address);
	/*do
	{
	std::cout << "Type the address or name of the server to connect to: ";
	std::cin >> server;
	} while (server == sf::IpAddress::None);*/

	// Create a socket for communicating with the server
	//sf::UdpSocket socket;
	socket->setBlocking(false);
	///////////////////////////////////////////
}

////////////////////////////////////////////////////////////
/// Send a message to the server, wait for the answer
///
////////////////////////////////////////////////////////////
void Network::runUdpClient()
{
	sf::Uint32 x = 24;
	std::string s = "hello";
	double d = 5.89;
	// Group the variables to send into a packet
	sf::Packet packet;
	packet << x << s << d;
	// Send it over the network (socket is a valid sf::TcpSocket)
	socket->send(packet, ip_address, *port);
	
		// Receive the packet at the other end
	sf::Packet packet;
	socket->receive(packet, ip_address, *port);
	// Extract the variables contained in the packet
	sf::Uint32 x;
	std::string s;
	double d;
	if (packet >> x >> s >> d)
	{
		// Data extracted successfully...
	}

	// Send a message to the server
	const char out[] = "Hi, I'm a client";

	switch (socket->send(out, sizeof(out), *address, *port))
	{
	case sf::Socket::NotReady:
		std::cout << "Socket not ready " << *address << std::endl;
		break;

	case sf::Socket::Done:
		std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
		break;

	case sf::Socket::Disconnected:
		std::cout << "Disconnected" << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "Socket Error" << std::endl;
		break;

	default:
		std::cout << "Default Error" << std::endl;
		return;
	}

	// Receive an answer from anyone (but most likely from the server)
	char in[128];
	std::size_t received;
	sf::IpAddress sender;
	unsigned short senderPort;
	switch (socket->receive(in, sizeof(in), received, sender, senderPort))
	{
	case sf::Socket::NotReady:
		std::cout << "Socket not ready " << *address << std::endl;
		break;

	case sf::Socket::Done:
		std::cout << "Message received from " << sender << ": \"" << in << "\"" << std::endl;
		break;

	case sf::Socket::Disconnected:
		std::cout << "Disconnected" << std::endl;
		break;

	case sf::Socket::Error:
		std::cout << "Socket Error" << std::endl;
		break;

	default:
		std::cout << "Default Error" << std::endl;
		return;
	}
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

void Network::update(float dt)
{
	//runUdpServer(port);

	// TCP, UDP or connected UDP ?
	/*char protocol;
	std::cout << "Do you want to use TCP (t) or UDP (u)? ";
	std::cin >> protocol;*/

	// Client or server ?
	//do_once();
	if (input->isKeyDown(sf::Keyboard::S))
	{
		input->setKeyUp(sf::Keyboard::S);
		networkState = NetworkState::SERVER;
		// create the socket
		serverSocket();
		//text.setPosition(200, 100);
		text.setString("Connecting...\n\nYou're the server\n\nPress Enter to Play");
	}
	if (input->isKeyDown(sf::Keyboard::C))
	{
		input->setKeyUp(sf::Keyboard::C);
		networkState = NetworkState::CLIENT;
		clientSocket();
		// message - joined the server
		//text.setPosition(200, 100);
		text.setString("Connecting...\n\nYou're the client\n\nPress Enter to Play");
	}
	//if (protocol == 't')
	//{
	//	// Test the TCP protocol
	//	if (who == 's')
	//		//runTcpServer(port);
	//		return;
	//	else
	//		//runTcpClient(port);
	//		return;
	//}
	//else
	//{
	// Test the unconnected UDP protocol

	// put into game
	switch (getNetworkState())
	{
	case NetworkState::SERVER:
		runUdpServer();
		break;
	case NetworkState::CLIENT:
		runUdpClient();
		break;
	case NetworkState::NONE:
		text.setString("Press 'S' to be a server\n\nPress 'C' to be a client\n\nPress Enter to Play");
		break;
	}
		
	//}

	//// Wait until the user presses 'enter' key
	//std::cout << "Press enter to exit..." << std::endl;
	//std::cin.ignore(10000, '\n');
	//std::cin.ignore(10000, '\n');
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
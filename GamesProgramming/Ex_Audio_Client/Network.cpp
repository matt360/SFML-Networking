#include "Network.h"
#include "SFML/Network.hpp"
#include <iostream>

NetworkSelect::NetworkSelect(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;
	state = GameStateEnum::NETWORK;

	// NetworkSelect text
	font.loadFromFile("font/arial.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Connecting...\n\nPress Enter to Play");
}

NetworkSelect::~NetworkSelect()
{
}

GameStateEnum NetworkSelect::getState()
{
	return state;
}

void NetworkSelect::handleInput(float dt)
{

	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		state = GameStateEnum::LEVEL;
	}
	else
	{
		state = GameStateEnum::NETWORK;
	}
}

////////////////////////////////////////////////////////////
/// Launch a server, wait for a message, send an answer.
///
////////////////////////////////////////////////////////////
void NetworkSelect::runUdpServer(unsigned short port)
{
	// Create a socket to receive a message from anyone
	sf::UdpSocket socket;
	socket.setBlocking(false);

	// Listen to messages on the specified port
	if (socket.bind(port) != sf::Socket::Done)
		return;
	std::cout << "Server is listening to port " << port << ", waiting for a message... " << std::endl;

	// Wait for a message
	char in[128];
	std::size_t received;
	sf::IpAddress sender;
	unsigned short senderPort;
	if (socket.receive(in, sizeof(in), received, sender, senderPort) != sf::Socket::Done)
		return;
	std::cout << "Message received from client " << sender << ": \"" << in << "\"" << std::endl;

	// Send an answer to the client
	const char out[] = "Hi, I'm the server";
	if (socket.send(out, sizeof(out), sender, senderPort) != sf::Socket::Done)
		return;
	std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;
}

////////////////////////////////////////////////////////////
/// Send a message to the server, wait for the answer
///
////////////////////////////////////////////////////////////
void NetworkSelect::runUdpClient(unsigned short port)
{
	// Ask for the server address
	sf::IpAddress server("127.1.0");
	/*do
	{
		std::cout << "Type the address or name of the server to connect to: ";
		std::cin >> server;
	} while (server == sf::IpAddress::None);*/

	// Create a socket for communicating with the server
	sf::UdpSocket socket;
	socket.setBlocking(false);

	// Send a message to the server
	const char out[] = "Hi, I'm a client";

	switch (socket.send(out, sizeof(out), server, port))
	{
	case sf::Socket::NotReady :
		std::cout << "Socket not ready " << server << std::endl;
		break;

	case sf::Socket::Done:
		std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
		break;

	case sf::Socket::Disconnected:
		std::cout << "Disconnected" << std::endl;
		break;

	case sf::Socket::Error :
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
	switch (socket.receive(in, sizeof(in), received, sender, senderPort))
	{
	case sf::Socket::NotReady:
		std::cout << "Socket not ready " << server << std::endl;
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

void NetworkSelect::update(float dt)
{
	runUdpClient(port);

	//// TCP, UDP or connected UDP ?
	//char protocol;
	//std::cout << "Do you want to use TCP (t) or UDP (u)? ";
	//std::cin >> protocol;

	//// Client or server ?
	//char who;
	//std::cout << "Do you want to be a server (s) or a client (c)? ";
	//std::cin >> who;

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
	//	// Test the unconnected UDP protocol
	//	if (who == 's')
	//		runUdpServer(port);
	//	else
	//		runUdpClient(port);
	//}

	//// Wait until the user presses 'enter' key
	//std::cout << "Press enter to exit..." << std::endl;
	//std::cin.ignore(10000, '\n');
	//std::cin.ignore(10000, '\n');
}

void NetworkSelect::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

void NetworkSelect::beginDraw()
{
	window->clear(sf::Color(100, 149, 237));
}

void NetworkSelect::endDraw()
{
	window->display();
}
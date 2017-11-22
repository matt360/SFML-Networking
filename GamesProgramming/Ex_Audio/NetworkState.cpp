#include "NetworkState.h"
#include <iostream>

NetworkState::NetworkState()
{
}

NetworkState::~NetworkState()
{
}


//void NetworkState::render()
//{
//	beginDraw();
//
//	window->draw(text);
//
//	endDraw();
//}

void NetworkState::displayReceiveMessage(sf::Int32 time)
{
	// Message FROM the server
	std::cout << "\n\nCLIENT: Message received from the server:";
	// Data extracted successfully...
	std::cout << "\nCLIENT: server's time: " << time;
}

void NetworkState::displaySendMessage(sf::Int32 time)
{
	// Message sent TO the server
	std::cout << "\n\nCLIENT: Message sent to the server:";
	// Data extracted successfully...
	std::cout << "\nCLIENT: client's time: " << time;
}

void NetworkState::displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port)
{
	// Message FROM the server
	std::cout << "\nCLIENT: IP: " << sender;
	std::cout << "\nCLIENT: port: " << sender_port;
	std::cout << "\nCLIENT: time: " << time;
}

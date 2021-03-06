#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <list>

class Network
{
public:
	Network();
	~Network();

protected:
	// socket
	static sf::UdpSocket socket;
	// Choose an arbitrary port for opening sockets
	unsigned short port = 50001;
	// IP adrress to connect to (local host IP by default)
	sf::IpAddress ip_address = "127.0.0.1";
	// Create a list to store the future clients
	std::list<sf::UdpSocket*> clients;
	// Create a selector
	sf::SocketSelector selector;

	// connection handler - does not let the server start without establishing the connection with the client
	bool established_connection;

	// get cthe current time and take the offset into account
	sf::Int32 getCurrentTime(const sf::Clock& clock, const sf::Int32& offset);

	// Debug functions
	void displayReceiveMessage(sf::Int32 time);
	void displaySendMessage(sf::Int32 time);
};


#pragma once
#include <SFML/Network.hpp>
#include <iostream>

class Network
{
public:
	Network(unsigned short pt = 50001, sf::IpAddress ip_adr = "127.1.0");
	~Network();

protected:
	// NETWORK /////////////////////
	// socket
	static sf::UdpSocket socket;
	// Choose an arbitrary port for opening sockets
	unsigned short port = 50001;
	// IP adrress to connect to
	sf::IpAddress ip_address = "127.1.0";
	// connection handler - does not let the server start without establishing the connection with the client
	bool established_connection = false;
	////////////////////////////////

	void displayReceiveMessage(sf::Int32 time);
	void displaySendMessage(sf::Int32 time);
	//void displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port);
};


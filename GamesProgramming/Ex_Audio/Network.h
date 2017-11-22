#pragma once
#include <SFML/Network.hpp>

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
	////////////////////////////////
};


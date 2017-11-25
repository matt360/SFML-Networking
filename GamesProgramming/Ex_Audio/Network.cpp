#include "Network.h"

Network::Network(unsigned short pt, sf::IpAddress ip_adr, bool es_con) : port(pt), ip_address(ip_adr), established_connection(es_con) {}

Network::~Network() {}

// REALLY IMPORTANT FOR STATIC CLASS MEMBERS!!!
// Without static definition out of the class declaration in
// *.cpp file a linking error will occur!
sf::UdpSocket Network::socket;

void Network::displayReceiveMessage(sf::Int32 time)
{
	// Message FROM the server
	std::cout << "\n\nCLIENT: Message received from the server:";
	// Data extracted successfully...
	std::cout << "\nCLIENT: server's time: " << time;
}

void Network::displaySendMessage(sf::Int32 time)
{
	// Message sent TO the server
	std::cout << "\n\nCLIENT: Message sent to the server:";
	// Data extracted successfully...
	std::cout << "\nCLIENT: client's time: " << time;
}

//void NetworkClient::displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port)
//{
//	// Message FROM the server
//	std::cout << "\nCLIENT: IP: " << sender;
//	std::cout << "\nCLIENT: port: " << sender_port;
//	std::cout << "\nCLIENT: time: " << time;
//}

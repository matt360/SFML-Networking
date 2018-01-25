#include "Network.h"

Network::Network() {}
Network::~Network() {}

// REALLY IMPORTANT FOR STATIC CLASS MEMBERS!!!
// Without static definition out of the class declaration in
// *.cpp file a linking error will occur!
sf::UdpSocket Network::socket;

sf::Int32 Network::getCurrentTime(const sf::Clock& clock, const sf::Int32& offset)
{
	sf::Int32 current_time = clock.getElapsedTime().asMilliseconds();
	return sf::Int32(current_time + (offset));
}

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

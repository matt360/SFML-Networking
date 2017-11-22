#include "Network.h"

Network::Network(unsigned short pt, sf::IpAddress ip_adr) : port(pt), ip_address(ip_adr) {}

Network::~Network() {}

// REALLY IMPORTANT FOR STATIC CLASS MEMBERS!!!
// Without static definition out of the class declaration in
// *.cpp file a linking error will occur!
sf::UdpSocket Network::socket;
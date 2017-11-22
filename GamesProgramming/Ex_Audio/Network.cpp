#include "Network.h"

Network::Network(unsigned short pt, sf::IpAddress ip_adr) : port(pt), ip_address(ip_adr) {}

Network::~Network() {}

sf::UdpSocket Network::socket;
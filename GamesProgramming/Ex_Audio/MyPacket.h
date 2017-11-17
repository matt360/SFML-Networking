#pragma once
#include <SFML/Network/Packet.hpp>
#include "Messages.h"

class MyPacket : public sf::Packet
{
public:
	MyPacket();
	~MyPacket();
};


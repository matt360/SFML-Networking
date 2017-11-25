#pragma once
#include "Network.h"
#include "State.h"

class NetworkServer : public Network, public State
{
public:
	NetworkServer();
	~NetworkServer();

	/////////////
	// NETWORK //
	/////////////
	// server
	sf::Packet groupIntoPacket();
	void receivePacket(sf::Packet& packet_receive);

	void establishConnectionWithClient(const bool& debug_mode);
};


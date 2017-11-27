#pragma once
#include "Network.h"
#include "State.h"

class NetworkServer : public Network, public State
{
public:
	NetworkServer();
	~NetworkServer();

protected:
	// server
	sf::Packet groupIntoPacket(const sf::Int32& lag);
	void receivePacket(sf::Packet& packet_receive);

	void establishConnectionWithClient(const sf::Int32& lag, bool& debug_mode);
};


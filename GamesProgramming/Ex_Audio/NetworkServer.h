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
	sf::Packet groupIntoPacket();
	void receivePacket(sf::Packet& packet_receive);

	sf::Packet groupIntoPacket(const bool & rd);
	void establishConnectionWithClient(const bool& debug_mode);
	void setReady(const bool& ready);
};


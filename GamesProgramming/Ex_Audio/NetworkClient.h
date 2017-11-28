#pragma once
#include <iostream>
#include "State.h"
#include "Network.h"

class NetworkClient : public Network, public State
{
public:
	NetworkClient();
	~NetworkClient();

protected:
	// client
	sf::Packet groupIntoPacket();
	void receivePacket(sf::Packet& packet_receive);

	sf::Int32 start_timing_latency;
	sf::Int32 end_timing_latency;
	sf::Int32 latency;

	bool send_packet = true;
	void sendPacketToServer(const bool& debug_mode);
	void checkForIncomingPacketsFromServer(const bool& debug_mode);
	void establishConnectionWithServer(const bool& debug_mode);
};


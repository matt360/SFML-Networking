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
	/////////////
	// NETWORK //
	/////////////
	// client
	sf::Int32 start_timing_latency;
	sf::Int32 end_timing_latency;
	sf::Int32 latency;
	sf::Int32 getCurrentTime();

	bool send_packet;

	void sendPacketToServer();
	void checkForIncomingPacketsFromServer();
	void establishConnectionWithServer();

	bool established_connection;
	bool debug_mode;
	bool debug_message;
};


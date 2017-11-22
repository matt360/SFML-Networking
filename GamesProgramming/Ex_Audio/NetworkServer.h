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
	void establishConnectionWithClient();

	bool established_connection;
	bool debug_mode;
	bool debug_message;
};


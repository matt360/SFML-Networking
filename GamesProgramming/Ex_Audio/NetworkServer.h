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
	bool established_connection;
	void establishConnectionWithClient(const bool& debug_mode);
};


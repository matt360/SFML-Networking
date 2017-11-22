#pragma once
#include "Network.h"

class NetworkSelect : public Network
{
public:
	NetworkSelect();
	~NetworkSelect();

	/////////////
	// NETWORK //
	/////////////
	// client
	void createClientSocket();

	// server
	void createServerSocket();
};


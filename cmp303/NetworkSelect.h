#pragma once
#include "Network.h"

class NetworkSelect : public Network
{
public:
	NetworkSelect();
	~NetworkSelect();

protected:
	// client
	void createClientSocket();

	// server
	void createServerSocket();
};


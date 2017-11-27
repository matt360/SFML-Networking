#pragma once
#include "NetworkServer.h"

class NetworkServerState : public NetworkServer
{
public:
	NetworkServerState(sf::RenderWindow* hwnd, Input* in);

	~NetworkServerState();

	virtual void handleInput();
	virtual void update();
	// virtual render function is the same for all Networking states and could be inherited from 
	// the NetworkClient base class but for the sake of clarity it's being repeted in the derived class
	virtual void render();

private:
	bool connection_status;
	bool debug_mode;
	bool debug_message;
};


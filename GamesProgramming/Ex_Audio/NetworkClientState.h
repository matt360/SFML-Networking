#pragma once
#include "NetworkClient.h"

class NetworkClientState : public NetworkClient
{
public:
	NetworkClientState(sf::RenderWindow* hwnd, Input* in);
	~NetworkClientState();

	virtual void handleInput();
	virtual void update();
	// virtual render function is the same for all Networking states and could be inherited from 
	// the NetworkClient base class but for the sake of clarity it's being repeted in the derived class
	virtual void render();

private:
	bool debug_mode;
	bool debug_message;
};


#pragma once
#include "Input.h"
#include "Network.h"
#include "NetworkState.h"

class NetworkServer : public NetworkState, public Network
{
public:
	NetworkServer(sf::RenderWindow* hwnd,
		Input* in,
		sf::Clock* clock,
		sf::Int32* offset);
	~NetworkServer();

	virtual void handleInput();
	virtual void update();
	// virtual render function is the same for all Networking states and could be inherited from 
	// the NetworkState base class but for the sake of clarity it's being repeted in the derived class
	virtual void render();

	/////////////
	// NETWORK //
	/////////////
	// server
	void establishConnectionWithClient();
};


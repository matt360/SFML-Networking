#pragma once
#include "Input.h"
#include "Network.h"
#include "State.h"

class NetworkServerState : public State, public Network
{
public:
	NetworkServerState(sf::RenderWindow* hwnd, Input* in);

	~NetworkServerState();

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


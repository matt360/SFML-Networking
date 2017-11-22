#pragma once
#include "Input.h"
#include "Network.h"
#include "NetworkState.h"

class NetworkSelectState : public State, public Network
{
public:
	NetworkSelectState(sf::RenderWindow* hwnd, Input* in);
	~NetworkSelectState();

	virtual void handleInput();
	virtual void update();
	// virtual render function is the same for all Networking states and could be inherited from 
	// the NetworkState base class but for the sake of clarity it's being repeted in the derived class
	// which also makes changes to in
	virtual void render();

	/////////////
	// NETWORK //
	/////////////
	// client
	void createClientSocket();

	// server
	void createServerSocket();	
};


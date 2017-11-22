#pragma once
#include "Network.h"
#include "NetworkState.h"

class NetworkSelect : public NetworkState, public Network
{
public:
	NetworkSelect(sf::RenderWindow* hwnd);
	~NetworkSelect();

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


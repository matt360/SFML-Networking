#pragma once
#include "Input.h"
#include "State.h"
#include "NetworkSelect.h"

class NetworkSelectState : public State, public NetworkSelect
{
public:
	NetworkSelectState(sf::RenderWindow* hwnd, Input* in);
	~NetworkSelectState();

	virtual void handleInput();
	virtual void update();
	// virtual render function is the same for all Networking states and could be inherited from 
	// the NetworkClient base class but for the sake of clarity it's being repeted in the derived class
	// which also makes changes to in
	virtual void render();
};


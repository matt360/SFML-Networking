#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "States.h"

class State
{
public:
	State();
	virtual ~State();
	
	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	void beginDraw();
	void endDraw();

	sf::RenderWindow* window;
	Input* input;
	GameState* state;
	NetworkState* network_state;
	// Network
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;
};


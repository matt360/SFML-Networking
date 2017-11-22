#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "State.h"

class NetworkSelect : public State
{
public:
	NetworkSelect(sf::RenderWindow* hwnd, 
		Input* in, 
		GameState* state, 
		NetworkState* network_state,
		sf::UdpSocket* udp_socket, 
		sf::IpAddress* ip, 
		unsigned short *port_number);
	~NetworkSelect();

	void handleInput();
	void update();
	void render();

	/////////////
	// NETWORK //
	/////////////
	// client
	void createClientSocket();

	// server
	void createServerSocket();	
};


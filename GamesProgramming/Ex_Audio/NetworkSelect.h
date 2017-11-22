#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "NetworkState.h"

class NetworkSelect : public NetworkState
{
public:
	NetworkSelect(sf::RenderWindow* hwnd, 
		Input* in, 
		GameStateEnum* game_state, 
		NetworkStateEnum* network_state,
		sf::UdpSocket* udp_socket, 
		sf::IpAddress* ip, 
		unsigned short *port_number);
	~NetworkSelect();

	virtual void handleInput();
	virtual void update();
	virtual void render();

	/////////////
	// NETWORK //
	/////////////
	// client
	void createClientSocket();

	// server
	void createServerSocket();	
};


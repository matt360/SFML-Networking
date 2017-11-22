#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "NetworkState.h"

class NetworkServer : public NetworkState
{
public:
	NetworkServer(sf::RenderWindow* hwnd,
		Input* in,
		GameStateEnum* game_state,
		NetworkStateEnum* network_state,
		sf::UdpSocket* udp_socket,
		sf::IpAddress* ip,
		unsigned short *port_number,
		sf::Clock* clock,
		sf::Int32* offset);
	~NetworkServer();

	virtual void handleInput();
	virtual void update();
	virtual void render();
	// virtual render function is the same for all Networking states so is defined in the NetworkState base class

	/////////////
	// NETWORK //
	/////////////
	// server
	void establishConnectionWithClient();
};


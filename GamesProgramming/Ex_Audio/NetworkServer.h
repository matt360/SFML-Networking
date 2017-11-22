#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "State.h"

class NetworkServer : public NetworkState
{
public:
	NetworkServer(sf::RenderWindow* hwnd,
		Input* in,
		GameStateEnum* state,
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

	/////////////
	// NETWORK //
	/////////////
	// server
	void establishConnectionWithClient();

	void displayReceiveMessage(sf::Int32 time);
	void displaySendMessage(sf::Int32 time);
	void displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port);
};


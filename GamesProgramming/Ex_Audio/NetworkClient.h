#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "NetworkState.h"

class NetworkClient : public NetworkState
{
public:
	NetworkClient(sf::RenderWindow* hwnd,
		Input* in,
		GameStateEnum* game_state,
		sf::UdpSocket* udp_socket,
		sf::IpAddress* ip,
		unsigned short *port_number,
		sf::Clock* clock,
		sf::Int32* offset);
	~NetworkClient();

	virtual void handleInput();
	virtual void update();
	// virtual render function is the same for all Networking states and could be inherited from 
	// the NetworkState base class but for the sake of clarity it's being repeted in the derived class
	virtual void render();

	/////////////
	// NETWORK //
	/////////////
	// client
	sf::Int32 start_timing_latency;
	sf::Int32 end_timing_latency;
	sf::Int32 latency;
	sf::Int32 getCurrentTime();

	bool send_packet;

	void sendPacketToServer();
	void checkForIncomingPacketsFromServer();
	void establishConnectionWithServer();
};


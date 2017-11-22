#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "State.h"

class NetworkClient : public State
{
public:
	NetworkClient(sf::RenderWindow* hwnd,
		Input* in,
		GameState* state,
		NetworkState* network_state,
		sf::UdpSocket* udp_socket,
		sf::IpAddress* ip,
		unsigned short *port_number,
		sf::Clock* clock,
		sf::Int32* offset);
	~NetworkClient();

	void handleInput();
	void update();
	void render();

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

	void displayReceiveMessage(sf::Int32 time);
	void displaySendMessage(sf::Int32 time);
	void displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port);
};


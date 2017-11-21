#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "States.h"

class NetworkClient
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
		sf::Int32* current_time);
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

	void sendPacketToServer();
	void checkForIncomingPacketsFromServer();
	void establishConnectionWithServer();

	void displayReceiveMessage(sf::Int32 time);
	void displaySendMessage(sf::Int32 time);
	void displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port);

private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	GameState* state;
	NetworkState* network_state;
	sf::Clock* clock;
	sf::Int32* current_time;
	// Network
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;

	bool established_connection;
	sf::Int32 client_time;
	sf::Int32 server_time;
	// input handlers
	bool readyToPlay;
	bool server;
	bool client;
	bool debug_mode;
	bool debug_message;

	sf::Text text;
	sf::Font font;
};


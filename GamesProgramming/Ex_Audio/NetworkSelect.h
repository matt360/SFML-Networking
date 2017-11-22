#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "State.h"
#include "States.h"

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
	
private:
	bool readyToConnect;
	bool server;
	bool client;

	//void beginDraw();
	//void endDraw();
	//sf::RenderWindow* window;
	//Input* input;
	//GameState* state;
	//NetworkState* network_state;
	//// Network
	//sf::UdpSocket *socket;
	//sf::IpAddress* ip_address;
	//unsigned short* port;

	// input handlers
	/*bool debug_mode;
	bool debug_message;*/

	/*sf::Text text;
	sf::Font font;*/
};


#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "States.h"

class Network
{
public:
	Network(sf::RenderWindow* hwnd, Input* in, GameState* state, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short *port_number);
	~Network();

	void handleInput(float dt);
	void update(float dt);
	void render();

	// Network
	void serverSocket();
	void clientSocket();
	
private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	GameState* state;
	// Network
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;
	// input handlers
	bool readyToPlay;
	bool server;
	bool client;
	NetworkState network_state;

	sf::Text text;
	sf::Font font;
};


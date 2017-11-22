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

protected:
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
	sf::Clock* clock;
	sf::Int32* offset;

	bool established_connection;
	//sf::Int32 client_time;
	//sf::Int32 server_time;
	//sf::Int32 server_offset;
	// input handlers
	bool readyToPlay;
	bool server;
	bool client;
	bool debug_mode;
	bool debug_message;

	sf::Text text;
	sf::Font font;
};


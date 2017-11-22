#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include <string.h>
#include "States.h"

class NetworkSelectState
{
public:
	NetworkSelectState(sf::RenderWindow* hwnd, Input* in);
	~NetworkSelectState();

	void handleInput(float dt);
	void update(float dt);
	void render();
	GameStateEnum getState();

	// Network
	const unsigned short port = 50001; // Choose an arbitrary port for opening sockets
	void runUdpServer(unsigned short port);
	void runUdpClient(unsigned short port);

private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	GameStateEnum game_state;

	sf::Text text;
	sf::Font font;


};


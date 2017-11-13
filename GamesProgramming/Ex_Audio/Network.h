#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include <string.h>
#include "States.h"
#include <mutex>

class Network
{
public:
	Network(sf::RenderWindow* hwnd, Input* in);
	~Network();

	void handleInput(float dt);
	void update(float dt);
	void render();
	GameState getState();

	// Network
	const unsigned short port = 50001; // Choose an arbitrary port for opening sockets
	void runUdpServer(unsigned short port);
	void runUdpClient(unsigned short port);
	
private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	GameState state;

	sf::Text text;
	sf::Font font;

	std::once_flag ask_flag;
	char who;
	void do_once();
};


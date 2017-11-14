#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "States.h"
#include <mutex>

class Network
{
public:
	Network(sf::RenderWindow* hwnd, Input* in, sf::UdpSocket* udp_socket, unsigned short *port_number, const std::string* server_address);
	~Network();

	void handleInput(float dt);
	void update(float dt);
	void render();
	GameState getState();

	// Network
	void serverSocket();
	void runUdpServer();
	
	void clientSocket();
	void runUdpClient();
	
private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	sf::UdpSocket *socket;
	unsigned short* port;
	const std::string* address;

	GameState state;
	//NetworkState networkState;

	sf::Text text;
	sf::Font font;

	std::once_flag ask_flag;
	char who;
	//void do_once();

	// Network
};


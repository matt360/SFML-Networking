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
	Network(sf::RenderWindow* hwnd, Input* in, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short *port_number);
	~Network();

	void handleInput(float dt);
	void update(float dt);
	void render();
	//GameState getState();

	// Network
	void serverSocket();
	void clientSocket();
	
private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;

	GameState state;
	//NetworkState networkState;

	sf::Text text;
	sf::Font font;

	std::once_flag ask_flag;
	char who;
	//void do_once();

	// Network
};


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
	Network(sf::RenderWindow* hwnd, Input* in);
	~Network();

	void handleInput(float dt);
	void update(float dt);
	void render();
	GameState getState();

	// Network
	const unsigned short port = 50001; // Choose an arbitrary port for opening sockets
	void serverSocket(unsigned short port);
	void runUdpServer(unsigned short port, sf::UdpSocket socket);
	
	const std::string server = "127.1.0";
	void clientSocket(const std::string& address);
	void runUdpClient(unsigned short port, sf::UdpSocket socket, const std::string& server);
	
private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	GameState state;
	//NetworkState networkState;

	sf::Text text;
	sf::Font font;

	std::once_flag ask_flag;
	char who;
	//void do_once();

	// Network
};


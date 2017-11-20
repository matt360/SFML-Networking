#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "States.h"

class Network
{
public:
	Network(sf::RenderWindow* hwnd, Input* in, GameState* state, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short *port_number, sf::Clock* clock, float* current_time);
	~Network();

	void handleInput();
	void update();
	void render();

	/////////////
	// NETWORK //
	/////////////
	// cient
	void createClientSocket();
	void sendPacketToServer();
	void checkForIncomingPacketsFromServer();
	void establishConnectionWithServer();

	void displayClientMessage(sf::Int32 time);
	void displayClientMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port);

	// server
	void createServerSocket();
	//void receivePacketFromClient();
	//void sendPacketToClient();
	void establishConnectionWithClient();

	void displayServerMessage(sf::Int32 time);
	void displayServerMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port);
	
private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	GameState* state;
	sf::Clock* clock;
	float* current_time;
	// Network
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;

	sf::Int32 server_receive_time;
	sf::Int32 server_send_time;

	sf::Int32 client_receive_time;
	sf::Int32 client_send_time;
	// input handlers
	bool readyToPlay;
	bool server;
	bool client;
	bool debug_mode;
	bool debug_message;
	NetworkState network_state;

	sf::Text text;
	sf::Font font;
};


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

	void handleInput();
	void update();
	void render();

	// Network
	void createServerSocket();
	void createClientSocket();

	void displayMessage();

	void displayMessage(const sf::IpAddress sender, const unsigned short sender_port);

	void addMessage();

	void sendPacket();

	void establishConnectionWithClient();
	void checkForIncomingPackets();
	void establishConnectionWithServer();
	
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
	float send_time;
	float receive_time;
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


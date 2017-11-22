#pragma once

#include "GameState.h"

class GameServer : public GameState {
public:
	GameServer(sf::RenderWindow* hwnd,
		Input* in,
		sf::UdpSocket* udp_socket,
		sf::IpAddress* ip,
		unsigned short *port_number,
		sf::Clock* clock,
		sf::Int32* offset);
	~GameServer();

	virtual void handleInput();
	virtual void update();
	virtual void render();
	//Window* getWindow();

private:
	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);
	//void moveMushroom();
	//Window m_window;
	sf::RenderWindow* window;
	Input* input;
	// Network //////////////////////
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;
	sf::Clock* clock;
	sf::Int32* offset;

	virtual void addMessage(PlayerMessage& player_message);
	void runUdpServer();

	// window

	void beginDraw();
	void endDraw();
	/////////////////////////////////

	
};
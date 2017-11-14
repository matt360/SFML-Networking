#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "Input.h"
#include <Windows.h>
#include <string.h>
#include <mutex>
#include "Player.h"
#include "AnimatedSprite.h"
#include "Map.h"
#include "States.h"
#include "AudioManager.h"

class GameServer {
public:
	GameServer(sf::RenderWindow* hwnd, Input* in, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short *port_number);
	~GameServer();

	void handleInput(float dt);
	void update(float dt);
	void render();
	GameState getState();
	//Window* getWindow();

private:
	void runUdpServer();
	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);
	//void moveMushroom();
	//Window m_window;
	sf::RenderWindow* window;
	Input* input;
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;

	GameState state;

	std::once_flag ask_flag;

	float fps;
	sf::Text text;
	sf::Font font;

	void beginDraw();
	void endDraw();

	AudioManager audioMgr;

	bool hasStarted;

	// Game Variables
	Player player;
	//Enemy enemy;
	//Cursor cursor;
	sf::Texture texture;

	Map level;

	// sound test
	sf::SoundBuffer buff;
	sf::Sound soun;
};
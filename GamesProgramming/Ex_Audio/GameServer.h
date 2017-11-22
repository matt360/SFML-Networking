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
#include "EnumStates.h"
#include "AudioManager.h"
#include "Messages.h"
#include "State.h"

class GameServer : public State {
public:
	GameServer(sf::RenderWindow* hwnd,
		Input* in,
		GameState* state,
		sf::UdpSocket* udp_socket,
		sf::IpAddress* ip,
		unsigned short *port_number,
		sf::Clock* clock,
		sf::Int32* offset);
	~GameServer();

	void handleInput();
	void update();
	void render();
	//Window* getWindow();

private:
	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);
	//void moveMushroom();
	//Window m_window;
	sf::RenderWindow* window;
	Input* input;
	GameState* state;
	// Network //////////////////////
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;
	sf::Clock* clock;
	sf::Int32* offset;

	void runUdpServer();
	void addMessage(PlayerMessage& player_message);
	void displayMessage(const PlayerMessage player_message);
	void displayMessage(const PlayerMessage player_message, const sf::IpAddress ip_address, const unsigned short port);

	// window
	std::once_flag set_window_flag;
	void call_once_set_window(sf::RenderWindow& window);
	void call_once_set_window();

	// debug
	bool debug_mode;
	bool debug_message;
	/////////////////////////////////

	float fps;
	sf::Text text;
	sf::Text error_text;
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

	// My game variables
	
};
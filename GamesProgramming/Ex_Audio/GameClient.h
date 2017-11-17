#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "Input.h"
#include <Windows.h>
#include <string.h>
#include "Player.h"
#include "AnimatedSprite.h"
#include "Map.h"
#include "States.h"
#include "AudioManager.h"
#include "Messages.h"

//sf::Packet& operator <<(sf::Packet& packet, const PlayerMessage& player_message)
//{
//	return packet << player_message.id << player_message.x << player_message.y << player_message.time;
//}
//
//sf::Packet& operator >> (sf::Packet& packet, PlayerMessage& player_message)
//{
//	return packet >> player_message.id >> player_message.x >> player_message.y >> player_message.time;
//}

class GameClient {
public:
	GameClient(sf::RenderWindow* hwnd, Input* in, GameState* state, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short *port_number);
	~GameClient();

	void handleInput(float dt);
	void update(float dt);
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
	void sendPacket();
	void checkForIncomingPackets();
	/////////////////////////////////

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

	// My game variables
	
};
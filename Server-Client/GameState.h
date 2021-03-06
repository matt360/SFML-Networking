#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <Windows.h>
#include <iostream>
#include <string.h>
#include "Input.h"
#include "Player.h"
#include "Enemy.h"
#include "Messages.h"
#include "State.h"

class GameState : public State
{
public:
	GameState();
	~GameState();

protected:
	unsigned short frame;

	bool hasStarted;

	// Game Variables
	Player player;
	Enemy enemy;
	//Cursor cursor;
	sf::Texture player_texture;
	sf::Texture enemy_texture;

	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);

	//virtual void addMessage(Message& player_message) = 0;
	void displayMessage(const Message player_message);
	void displayMessage(const Message player_message, const sf::IpAddress ip_address, const unsigned short port);
};

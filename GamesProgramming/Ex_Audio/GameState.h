#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <Windows.h>
#include <iostream>
#include <string.h>
#include "Input.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "Map.h"
#include "AudioManager.h"
#include "Messages.h"
#include "State.h"

class GameState : public State
{
public:
	GameState();
	~GameState();

protected:
	float fps;

	AudioManager audioMgr;

	bool hasStarted;

	// Game Variables
	Player player;
	Player enemy;
	//Cursor cursor;
	sf::Texture texture;

	Map level;

	// sound test
	sf::SoundBuffer buff;
	sf::Sound soun;

	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);

	//virtual void addMessage(PlayerMessage& player_message) = 0;
	void displayMessage(const PlayerMessage player_message);
	void displayMessage(const PlayerMessage player_message, const sf::IpAddress ip_address, const unsigned short port);
};

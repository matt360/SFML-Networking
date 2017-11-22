#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <Windows.h>
#include <iostream>
#include <string.h>
#include <mutex>
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

	//virtual void render();

protected:
	float fps;

	sf::Text error_text;

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

	virtual void addMessage(PlayerMessage& player_message) = 0;
	void displayMessage(const PlayerMessage player_message);
	void displayMessage(const PlayerMessage player_message, const sf::IpAddress ip_address, const unsigned short port);
};

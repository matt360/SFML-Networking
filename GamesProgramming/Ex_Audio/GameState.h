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

	// TODO delete ///////////////////////////////////////
	std::once_flag set_window_flag;
	//void call_once_set_window(sf::RenderWindow& window);
	void call_once_set_window(sf::Vector2i vec);
	///////////////////////////////////////////////////////

	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);

	//virtual void addMessage(PlayerMessage& player_message) = 0;
	void displayMessage(const PlayerMessage player_message);
	void displayMessage(const PlayerMessage player_message, const sf::IpAddress ip_address, const unsigned short port);
};

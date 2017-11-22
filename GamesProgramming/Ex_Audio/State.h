#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "Input.h"

enum class NetworkStateEnum { CLIENT, SERVER, NONE };

enum class GameStateEnum { MENU, NETWORK, NETWORK_CLIENT, NETWORK_SERVER, GAME_CLIENT, GAME_SERVER, PAUSE, CREDITS };

class State
{
public:
	// Do NOT inline this constructor! Will couse linking errors
	State();
	virtual ~State();

	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	inline GameStateEnum getGameState() { return game_state; };
protected:
	sf::RenderWindow* window;
	Input* input;
	// NEVER RESTART THE CLOCK!!!
	static sf::Clock clock;
	static sf::Int32 offset;
	GameStateEnum game_state;
	NetworkStateEnum network_state;

	inline void beginDraw() { window->clear(sf::Color(0, 0, 0)); };
	inline void endDraw() { window->display(); };

	// input handlers
	bool ready;
	bool server;
	bool client;

	// display text
	sf::Text text;
	sf::Font font;
};


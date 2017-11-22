#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include <string.h>
#include "NetworkState.h"

class Menu : public NetworkState
{
public:
	Menu(sf::RenderWindow* hwnd, Input* in, GameStateEnum* state);
	~Menu();

	void handleInput();
	void update();
	void render();

private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	GameStateEnum* state;
	
	sf::Text text;
	sf::Font font;
};


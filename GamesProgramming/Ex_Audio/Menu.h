#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include <string.h>
#include "State.h"

class Menu : public NetworkState
{
public:
	Menu(sf::RenderWindow* hwnd, Input* in, GameState* state);
	~Menu();

	void handleInput();
	void update();
	void render();

private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	GameState* state;
	
	sf::Text text;
	sf::Font font;
};


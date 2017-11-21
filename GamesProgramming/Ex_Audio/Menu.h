#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include <string.h>
#include "State.h"
#include "States.h"

class Menu : public State
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


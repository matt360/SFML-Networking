#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include <string.h>
#include "States.h"

class Menu
{
public:
	Menu(sf::RenderWindow* hwnd, Input* in);
	~Menu();

	void handleInput(float dt);
	void update(float dt);
	void render();
	//GameState getState();

private:
	void beginDraw();
	void endDraw();
	sf::RenderWindow* window;
	Input* input;
	//GameState state;
	
	sf::Text text;
	sf::Font font;

	
};


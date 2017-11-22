#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include <string.h>
#include "State.h"

class Menu : public State
{
public:
	Menu(sf::RenderWindow* hwnd, Input* in);
	~Menu();

	virtual void handleInput();
	virtual void update();
	virtual void render();
};


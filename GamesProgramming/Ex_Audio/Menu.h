#pragma once
#include "NetworkState.h"

class Menu : public NetworkState
{
public:
	Menu(sf::RenderWindow* hwnd);
	~Menu();

	void handleInput();
	void update();
	void render();

private:
	void beginDraw();
	void endDraw();
	
	sf::Text text;
	sf::Font font;
};


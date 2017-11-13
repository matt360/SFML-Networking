#pragma once

#include <SFML/Graphics.hpp>
#include "Input.h"
#include <Windows.h>
#include <string.h>

class Game {
public:
	Game(sf::RenderWindow* hwnd, Input* in);
	~Game();

	void handleInput();
	void update();
	void render();
	//Window* getWindow();


private:
	//void moveMushroom();
	//Window m_window;
	sf::RenderWindow* window;
	Input* input;
	sf::Font font;
	sf::Text text;
	void beginDraw();
	void endDraw();

	// Game Variables
	sf::RectangleShape rect;
	sf::CircleShape circle;
	bool drag;
	sf::Vector2i startPos;
	sf::Vector2i endPos;
	
};
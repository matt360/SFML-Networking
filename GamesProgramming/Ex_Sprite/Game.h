#pragma once

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string.h>

#include "Sprite.h"

class Game {
public:
	Game(sf::RenderWindow* hwnd);
	~Game();

	void handleInput(float dt);
	void update(float dt);
	void render();
	//Window* getWindow();


private:
	//void moveMushroom();
	//Window m_window;
	sf::RenderWindow* window;
	sf::Font font;
	sf::Text text;
	void beginDraw();
	void endDraw();

	// Game Variables
	sf::RectangleShape rect;
	sf::Texture texture;
	sf::CircleShape circle;
	bool drag;
	sf::Vector2i startPos;
	sf::Vector2i endPos;

	sf::Vector2f position;
	Sprite mySprite;
};
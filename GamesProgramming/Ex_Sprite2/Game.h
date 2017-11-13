#pragma once

#include <SFML/Graphics.hpp>
#include "Input.h"
#include <Windows.h>
#include <string.h>
#include "Player.h"
#include "Enemy.h"
#include "Cursor.h"
#include "StaticSprite.h"

class Game {
public:
	Game(sf::RenderWindow* hwnd, Input* in);
	~Game();

	void handleInput(float dt);
	void update(float dt);
	void render();
	//Window* getWindow();


private:
	//void moveMushroom();
	//Window m_window;
	sf::RenderWindow* window;
	Input* input;

	void beginDraw();
	void endDraw();

	// Game Variables
	Player player;
	Enemy enemy;
	Cursor cursor;
	sf::Texture texture;
	sf::Texture enemyTexture;
	sf::Texture cursorTexture;

	sf::RectangleShape m_mushroom;
	sf::RectangleShape m_mushroom2;
	sf::Texture m_mushroomTexture;
	sf::Texture m_mushroomTexture2;


};
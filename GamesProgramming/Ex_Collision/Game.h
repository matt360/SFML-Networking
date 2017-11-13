#pragma once

#include <SFML/Graphics.hpp>
#include "Input.h"
#include <Windows.h>
#include <string.h>
#include "Player.h"
//#include "Enemy.h"
//#include "Cursor.h"
//#include "StaticSprite.h"
#include "AnimatedSprite.h"
//#include "Mario.h"
#include "Ball.h"
#include "Ball2.h"
#include "Ball3.h"
#include "Ball4.h"
#include "Paddle.h"
#include "Square1.h"

class Game {
public:
	Game(sf::RenderWindow* hwnd, Input* in);
	~Game();

	void handleInput(float dt);
	void update(float dt);
	void render();
	//Window* getWindow();


private:
	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);
	//void moveMushroom();
	//Window m_window;
	sf::RenderWindow* window;
	Input* input;

	void beginDraw();
	void endDraw();

	// Game Variables
	Player player;
	//Enemy enemy;
	//Cursor cursor;
	sf::Texture texture;
	sf::Texture enemyTexture;
	sf::Texture cursorTexture;


	AnimatedSprite zombie;
	sf::Texture zombTexture;


	//Mario mario;
	sf::Texture marioTexture;

	Ball ball;
	sf::Texture ballTexture;

	Ball2 ball2;

	Ball3 ball3;

	// pong example
	Paddle paddle;
	Ball4 ball4;

	Square1 square1;
	Square1 square2;
};
#include "Game.h"

Game::Game(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;
	
	texture.loadFromFile("gfx/Mushroom.png");

	player.setSize(sf::Vector2f(100, 100));
	player.setTexture(&texture);
	player.setPosition(100, 100);
	player.setInput(input);
	player.setVelocity(100, 0);

	enemyTexture.loadFromFile("gfx/Goomba.png");
	enemy.setSize(sf::Vector2f(50, 50));
	enemy.setTexture(&enemyTexture);
	enemy.setPosition(0, 200);

	cursorTexture.loadFromFile("gfx/icon.png");
	cursor.setTexture(&cursorTexture);
	cursor.setSize(sf::Vector2f(25, 25));
	cursor.setInput(input);

	window->setMouseCursorVisible(false);


	// Transparent sprite
	//m_mushroomTexture.loadFromFile("gfx/MushroomTrans1.png");
	//m_mushroom.setTexture(&m_mushroomTexture);
	//m_mushroom.setSize(sf::Vector2f(100, 100));
	//m_mushroom.setPosition(300, 300);





	sf::Image mask;
	mask.loadFromFile("gfx/MushroomMask.png");
	mask.createMaskFromColor(sf::Color::Magenta);

	m_mushroomTexture2.loadFromImage(mask);

	m_mushroom.setTexture(&m_mushroomTexture2);
	m_mushroom.setSize(sf::Vector2f(100, 100));
	m_mushroom.setPosition(300, 300);

}

Game::~Game()
{

}

void Game::update(float dt)
{
	enemy.update(dt);
	cursor.update(dt);
}

void Game::handleInput(float dt)
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.

	player.update(dt);

}


void Game::render()
{
	beginDraw();
	
	//window->draw(player);
	//window->draw(enemy);
	//window->draw(cursor);

	window->draw(m_mushroom);

	endDraw();
}

void Game::beginDraw()
{
	window->clear(sf::Color::Green);
}

void Game::endDraw()
{
	window->display();
}
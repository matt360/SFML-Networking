#include "Game.h"

Game::Game(sf::RenderWindow* hwnd)
{
	window = hwnd;

	rect.setSize(sf::Vector2f(50, 50));
	rect.setPosition(100, 100);
	rect.setFillColor(sf::Color::Red);

	circle.setRadius(15);
	circle.setPosition(300, 300);
	circle.setFillColor(sf::Color::Green);
	circle.setOutlineColor(sf::Color::Magenta);
	circle.setOutlineThickness(2.f);

	// setup class members
	//m_mushroomTexture.loadFromFile("gfx/MushroomTrans.png");
	//m_mushroom.setTexture(m_mushroomTexture);
	//increment = sf::Vector2f(0.1f, 0.1f);

	//sf::Image mask;
	//mask.loadFromFile("gfx/MushroomMask.png");
	//mask.createMaskFromColor(sf::Color::Magenta);

	////sf::Texture texture;
	//m_mushroomTexture2.loadFromImage(mask);

	////sf::Sprite m_mushroom2;
	//m_mushroom.setTexture(m_mushroomTexture2);
}

Game::~Game()
{

}

void Game::update()
{
	
}

void Game::handleInput()
{

}


void Game::render()
{
	beginDraw();
	
	window->draw(rect);
	window->draw(circle);

	endDraw();
}

void Game::beginDraw()
{
	window->clear(sf::Color::Black);
}

void Game::endDraw()
{
	window->display();
}
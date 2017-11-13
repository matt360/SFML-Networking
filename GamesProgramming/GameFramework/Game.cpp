#include "Game.h"

Game::Game() : m_window("Chapter 2", sf::Vector2u(800, 600))
{
	// setup class members
	m_mushroomTexture.loadFromFile("gfx/MushroomTrans.png");
	m_mushroom.setTexture(m_mushroomTexture);
	increment = sf::Vector2f(0.1f, 0.1f);

	sf::Image mask;
	mask.loadFromFile("gfx/MushroomMask.png");
	mask.createMaskFromColor(sf::Color::Magenta);

	//sf::Texture texture;
	m_mushroomTexture2.loadFromImage(mask);

	//sf::Sprite m_mushroom2;
	m_mushroom.setTexture(m_mushroomTexture2);
}

Game::~Game()
{

}

void Game::update()
{
	m_window.update();
	moveMushroom();
}

void Game::handleInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_isDone = true;
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
		{
			toggleFullscreen();
		}
	}
}

void Game::moveMushroom()
{
	sf::Vector2u l_windowSize = m_window.getWindowSize();
	sf::Vector2u l_textSize = m_mushroomTexture.getSize();

	if ((m_mushroom.getPosition().x > l_windowSize.x - l_textSize.x && increment.x > 0) || (m_mushroom.getPosition().x < 0 && increment.x < 0))
	{
		increment.x = -increment.x;
	}

	if ((m_mushroom.getPosition().y > l_windowSize.y - l_textSize.y && increment.y > 0) || (m_mushroom.getPosition().y < 0 && increment.y < 0))
	{
		increment.y = -increment.y;
	}

	m_mushroom.setPosition(m_mushroom.getPosition().x + increment.x, m_mushroom.getPosition().y + increment.y);
}

void Game::render()
{
	m_window.beginDraw();
	m_window.draw(m_mushroom);
	m_window.endDraw();
}

Window* Game::getWindow()
{
	return &m_window;
}
#include "Game.h"

Game::Game(sf::RenderWindow* hwnd)
{
	window = hwnd;
	//Load font
	if (!font.loadFromFile("font/arial.ttf"))
	{
		// something went wrong
		MessageBox(NULL, L"Font failed to load", L"Error", MB_OK);
	}

	drag = false;

	// Create a text
	text.setString("Hello world");
	text.setFont(font);
	text.setCharacterSize(30);
	text.setStyle(sf::Text::Bold);
	//text.setPosition(10, 10);
	text.setFillColor(sf::Color::Red);

	texture.loadFromFile("gfx/MushroomTrans.png");
	rect.setSize(sf::Vector2f(50, 50));
	rect.setPosition(100, 100);
	//rect.setFillColor(sf::Color::Red);
	rect.setTexture(&texture);

	circle.setRadius(15);
	circle.setPosition(300, 300);
	circle.setFillColor(sf::Color::Green);
	circle.setOutlineColor(sf::Color::Magenta);
	circle.setOutlineThickness(2.f);

}

Game::~Game()
{

}

void Game::update(float dt)
{
	std::string xpos = std::to_string(sf::Mouse::getPosition(*window).x);
	std::string ypos = std::to_string(sf::Mouse::getPosition(*window).y);
	//text.setString("Mouse: " + xpos + ", " + ypos);

	// Update position based on delta time
	position.x += dt*50.f;
	// without delta time
	//position.x += 0.1f;
	position.y = 300;
	circle.setPosition(position);

}

void Game::handleInput(float dt)
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		// space is pressed
		MessageBox(NULL, L"W was pressed", L"Key press", MB_OK);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		// Left mouse button pressed
		MessageBox(NULL, L"Right click", L"Mouse press", MB_OK);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		// reposition mouse cursor
		sf::Mouse::setPosition(sf::Vector2i(400, 300), *window);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !drag)
	{
		// Left mouse button pressed
		//MessageBox(NULL, L"Right click", L"Mouse press", MB_OK);
		startPos = sf::Mouse::getPosition(*window);
		drag = true;
	}
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && drag)
	{
		// Left mouse button pressed
		//MessageBox(NULL, L"Right click", L"Mouse press", MB_OK);
		endPos = sf::Mouse::getPosition(*window);
		drag = false;
		std::string diffx = std::to_string(endPos.x - startPos.x);
		std::string diffy = std::to_string(endPos.y - startPos.y);
		std::string distance = "diffx = " + diffx + ", diffy = " + diffy;
		text.setString(distance);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		// Left arrow is pressed
		rect.setPosition(rect.getPosition().x - (dt*100.f), rect.getPosition().y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		// Right arrow key is pressed
		rect.setPosition(rect.getPosition().x + (dt*100.f), rect.getPosition().y);
	}

}


void Game::render()
{
	beginDraw();
	
	window->draw(rect);
	window->draw(circle);
	window->draw(text);
	//mySprite.render(window);
	window->draw(mySprite.sprite);
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
#include "Game.h"

Game::Game(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;
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


	rect.setSize(sf::Vector2f(50, 50));
	rect.setPosition(100, 100);
	rect.setFillColor(sf::Color::Red);



	circle.setRadius(15);
	circle.setPosition(300, 300);
	circle.setFillColor(sf::Color::Green);
	circle.setOutlineColor(sf::Color::Magenta);
	circle.setOutlineThickness(2.f);

	step = 150.f;





	speed = 100.f;
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
	if (position.x + 30 > window->getSize().x)
	{
		step = step *-1;
	}
	else if (position.x < 0)
	{
		step = step * -1;
	}
	//position.x += dt*step;
	// without delta time
	//position.x += 0.1f;
	position.x = 700.f;
	position.y = 300;
	circle.setPosition(position);


	// Update position based on delta time
	//position.x += dt*step;
	circle.setPosition(position);


	// without delta time
	//position.x += 0.1f;

}

void Game::handleInput(float dt)
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.

	if (input->isKeyDown(sf::Keyboard::Left))
	{
		// Left arrow is pressed
		rect.setPosition(rect.getPosition().x - (dt*speed), rect.getPosition().y);
	}
	if (input->isKeyDown(sf::Keyboard::Right))
	{
		// Right arrow key is pressed
		rect.setPosition(rect.getPosition().x + (dt*speed), rect.getPosition().y);
	}
	if (input->isKeyDown(sf::Keyboard::Add))
	{
		speed += 20.f;
		input->setKeyUp(sf::Keyboard::Add);
	}
	if (input->isKeyDown(sf::Keyboard::Subtract))
	{
		speed -= 20.f;
		input->setKeyUp(sf::Keyboard::Subtract);
	}

}


void Game::render()
{
	beginDraw();

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
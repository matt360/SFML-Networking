#include "Menu.h"

Menu::Menu(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;
	//state = GameState::MENU;

	// Menu text
	font.loadFromFile("font/arial.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("MY FIRST GAME\n\nPress Enter to Play");
}

Menu::~Menu()
{
}

void Menu::handleInput(float dt)
{

	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		state = GameState::NETWORK;
	}
	else
	{
		state = GameState::MENU;
	}
}

void Menu::update(float dt)
{

}

void Menu::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

void Menu::beginDraw()
{
	window->clear(sf::Color(100, 149, 237));
}

void Menu::endDraw()
{
	window->display();
}
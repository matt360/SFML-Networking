#include "MenuState.h"

Menu::Menu(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	// Menu text
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("THE GREAT MUSHROOM CHASE\n\nNOW WITH OBSERVER MODE!\n\nPress Enter to Proceed");
}

Menu::~Menu()
{
}

void Menu::handleInput()
{

	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		game_state = GameStateEnum::NETWORK;
	}
	else
	{
		game_state = GameStateEnum::MENU;
	}
}

void Menu::update()
{

}

void Menu::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}
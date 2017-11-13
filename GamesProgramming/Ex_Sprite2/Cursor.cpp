#include "Cursor.h"

Cursor::Cursor(const sf::Vector2f & size) : Sprite(size)
{
	
}

Cursor::~Cursor()
{}

void Cursor::update(float dt)
{
	setPosition(input->getMouseX(), input->getMouseY());
}

void Cursor::setInput(Input* in)
{
	input = in;
}
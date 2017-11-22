#include "State.h"

State::State()
{
}

State::~State()
{
}

void State::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void State::endDraw()
{
	window->display();
}

#include "GameState.h"

GameState::GameState()
{

}

GameState::~GameState()
{

}

void GameState::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void GameState::endDraw()
{
	window->display();
}
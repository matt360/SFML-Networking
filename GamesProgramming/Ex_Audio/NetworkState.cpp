#include "NetworkState.h"

NetworkState::NetworkState()
{
}

NetworkState::~NetworkState()
{
}

void NetworkState::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void NetworkState::endDraw()
{
	window->display();
}

#pragma once
#include "State.h"
#include "Network.h"
#include "Messages.h"
#include "GameState.h"

class GameClient 
{
public:
	GameClient();
	~GameClient();

	sf::Int32 getCurrentTime();

};

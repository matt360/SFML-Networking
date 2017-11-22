#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include "State.h"

class GameState : public State
{
public:
	GameState();
	~GameState();
};

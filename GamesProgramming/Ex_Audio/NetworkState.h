#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include "State.h"

class NetworkState : public State
{
public:
	NetworkState();
	~NetworkState();
};


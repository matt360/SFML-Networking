#pragma once
#include "State.h"

class StateHandler : public State
{
public:
	StateHandler();
	~StateHandler();

	void handleInput() { return; };
	void update() { return; };
	void render() { return; };
};


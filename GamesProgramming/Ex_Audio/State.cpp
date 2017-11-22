#include "State.h"

State::State() 
{
	game_state = GameStateEnum::MENU;
}

State::~State() {}

// REALLY IMPORTANT FOR STATIC CLASS MEMBERS!!!
// Without static definition out of the class declaration in
// *.cpp file a linking error will occur!
sf::Int32 State::offset = 0;
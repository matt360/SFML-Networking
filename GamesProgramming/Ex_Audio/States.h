#pragma once
#include <SFML/Network.hpp>

enum class GameState { MENU, NETWORK, GAME_CLIENT, GAME_SERVER, PAUSE, CREDITS };
//enum class NetworkState { CLIENT, SERVER, NONE };

//__declspec(selectany) NetworkState networkState = NetworkState::CLIENT;
// Set initial state
__declspec(selectany) GameState state = GameState::MENU;

inline GameState getState() { return state; }

//inline NetworkState getNetworkState() { return networkState; }

//enum direction : char { left = 'l', right = 'r' };
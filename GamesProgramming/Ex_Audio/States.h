#pragma once
#include <SFML/Network.hpp>

enum class GameState { MENU, NETWORK, GAME_CLIENT, GAME_SERVER, PAUSE, CREDITS };
enum class NetworkState { CLIENT, SERVER, NONE };

__declspec(selectany) NetworkState networkState;
// Set initial state
__declspec(selectany) GameState state = GameState::MENU;

inline NetworkState getNetworkState() { return networkState; }

//enum direction : char { left = 'l', right = 'r' };
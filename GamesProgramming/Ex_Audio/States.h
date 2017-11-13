#pragma once

enum class GameState { MENU, NETWORK, LEVEL, PAUSE, CREDITS };
enum class NetworkState { CLIENT, SERVER };

__declspec(selectany) NetworkState networkState;

inline NetworkState getNetworkState() { return networkState; }
//enum direction : char { left = 'l', right = 'r' };
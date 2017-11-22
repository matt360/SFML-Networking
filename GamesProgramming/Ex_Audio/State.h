#pragma once

enum class NetworkStateEnum { CLIENT, SERVER, NONE };

enum class GameStateEnum { MENU, NETWORK, NETWORK_CLIENT, NETWORK_SERVER, GAME_CLIENT, GAME_SERVER, PAUSE, CREDITS };

class State
{
public:
	State() {}
	virtual ~State() {}

	sf::RenderWindow* window;
	Input* input;
	// Network
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;
	sf::Clock* clock;
	sf::Int32* offset;
	GameStateEnum* game_state;
	NetworkStateEnum* network_state;

	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
};


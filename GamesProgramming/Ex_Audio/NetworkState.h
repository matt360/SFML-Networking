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

protected:
	void displayReceiveMessage(sf::Int32 time);
	void displaySendMessage(sf::Int32 time);
	void displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port);
};


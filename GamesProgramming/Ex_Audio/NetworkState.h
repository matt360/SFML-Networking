#pragma once
#include "Input.h"
#include "Network.h"
#include "State.h"

class NetworkState : public State, public Network
{
public:
	NetworkState();
	~NetworkState();

protected:
	void displayReceiveMessage(sf::Int32 time);
	void displaySendMessage(sf::Int32 time);
	void displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port);
};


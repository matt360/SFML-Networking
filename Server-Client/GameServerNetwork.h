#pragma once
#include "State.h"
#include "Network.h"
#include "Messages.h"
#include "GameState.h"

class GameServerNetwork : public Network
{
public:
	GameServerNetwork();
	~GameServerNetwork();

protected:
	// toggle lerp mode - input handler
	bool lerp_mode;
	// toggle linear prediction - input handler
	bool linear_prediction;
	// toggle quadratic prediction - input handler
	bool quadratic_prediction;

	virtual void addMessage(Message& player_message_send, const Sprite& player, const Sprite& enemy, const sf::Clock& clock);
	// SEND (to the client)
	sf::Packet groupIntoPacket(const Message & player_message_send);
	// RECEIVE (from the client)
	void receivePacket(sf::Packet& packet_receive);

	// receive and send messages to the client (in that order)
	void sendMessageToClient(const Sprite& player, const Sprite& enemy, const sf::Clock& clock, const unsigned short& port, const bool& debug_mode);
};

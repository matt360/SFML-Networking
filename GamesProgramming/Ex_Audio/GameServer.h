#pragma once
#include "State.h"
#include "Network.h"
#include "Messages.h"
#include "GameState.h"

class GameServer : public Network
{
public:
	GameServer();
	~GameServer();

protected:
	// toggle lerp mode - input handler
	bool lerp_mode;
	// toggle linear prediction - input handler
	bool linear_prediction;
	// toggle quadratic prediction - input handler
	bool quadratic_prediction;

	virtual void addMessage(PlayerMessage& player_message_send, const Player& player, const sf::Clock& clock);
	// MATCH 1 - create a packet to be sent
	sf::Packet groupIntoPacket(const PlayerMessage & player_message_send);
	// MATCH 2 - not used for anything at the time (could be used for multiple clients) TODO 
	void receivePacket(sf::Packet& packet_receive);

	// receive and send messages to the client (in that order)
	void runUdpServer(const Player& player, const sf::Clock& clock, const bool& debug_mode);
};

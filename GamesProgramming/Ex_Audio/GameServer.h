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
	virtual void addMessage(PlayerMessage& player_message_send, const Player& player, const sf::Clock& clock);
	sf::Packet groupPacket(const PlayerMessage & player_message_send, const bool & linear_prediction, const bool & quadratic_prediction);

	void runUdpServer(const Player& player, const sf::Clock& clock, const bool& debug_mode);

	void receivePacket(sf::Packet& packet_receive);

	bool linear_prediction;
	bool quadratic_prediction;
};

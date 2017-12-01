#pragma once
#include <queue>
#include <deque>
#include <vector>
#include "State.h"
#include "Network.h"
#include "Messages.h"
#include "GameState.h"
#include "LinearPrediction.h"
#include "QuadraticPrediction.h"

// TODO change the name to GameClientNetowrk
class GameClientNetwork : public Network
{
public:
	GameClientNetwork();
	~GameClientNetwork();

protected:
	// toggle lerp mode - input handler
	bool lerp_mode;
	// toggle linear prediction - input handler
	bool linear_prediction;
	// toggle quadratic prediction - input handler
	bool quadratic_prediction;

	LinearPrediction player_linear_prediction;
	QuadraticPrediction player_quadratic_prediction;

	LinearPrediction enemy_linear_prediction;

	// get current time and take into account the offset from the server
	// compose a message to be send with Message structure
	void addMessage(Message& player_message, const Sprite& player, const Sprite& enemy, const sf::Clock& clock, const sf::Int32& offset);

	// RECEIVE (from the server's perspective what client is sending for the server to receive) 
	sf::Packet groupIntoPacket(const Message& player_message_send);
	// SEND (from the client's perspective what server is sending for the client to receive)
	void receivePacket(sf::Packet& packet_receive);

	// send packet
	void sendPacket(const Sprite& player, const Sprite& enemy, const sf::Clock& clock, const sf::Int32& offset, const bool& debug_mode);
	void checkForIncomingPackets(const bool& debug_mode);
};

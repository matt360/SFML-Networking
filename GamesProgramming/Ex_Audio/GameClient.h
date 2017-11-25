#pragma once
#include <queue>
#include <deque>
#include <vector>
#include "State.h"
#include "Network.h"
#include "Messages.h"
#include "GameState.h"

class GameClient : public Network
{
public:
	GameClient();
	~GameClient();

	// toggle lerp mode - input handler
	bool lerp_mode;
	// toggle linear prediction - input handler
	bool linear_prediction;
	// toggle quadratic prediction - input handler
	bool quadratic_prediction;

	// lerp functions
	float lerp(float start, float end, float time);
	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float& time);

	// get current time and take into account the offset from the server
	sf::Int32 getCurrentTime(const sf::Clock& clock, const sf::Int32& offset);
	// compose a message to be send with PlayerMessage structure
	void addMessage(PlayerMessage& player_message, const Player& player, const sf::Clock& clock, const sf::Int32& offset);

	/////////////////////////////////
	// LINEAR PREDICTION FUNCTIONS //
	/////////////////////////////////
	// number of messages to keep track of
	const unsigned int linear_message_number = 2;
	std::queue<PlayerMessage> linear_local_positions;
	void keepTrackOfLinearLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfLinearNetworkPositions(const PlayerMessage& player_message_receive);
	std::queue<PlayerMessage> linear_network_positions;
	sf::Vector2f predictLinearLocalPath(const sf::Clock& clock, const sf::Int32& offset);
	sf::Vector2f predictLinearNetworkPath(const sf::Clock& clock, const sf::Int32& offset);

	////////////////////////////////////
	// QUADRATIC PREDICTION FUNCTIONS //
	// /////////////////////////////////
	// number of messages to keep track of
	const unsigned int quadratic_message_number = 3;
	std::deque<PlayerMessage> quadratic_local_positions;
	void keepTrackOfQuadraticLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfQuadraticLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfQuadraticNetworkPositions(const PlayerMessage& player_message_receive);
	std::deque<PlayerMessage> quadratic_network_positions;
	sf::Vector2f predictQuadraticLocalPath(const sf::Clock & clock, const sf::Int32 & offset);
	sf::Vector2f predictQuadraticNetworkPath(const sf::Clock & clock, const sf::Int32 & offset);
	void linearInterpolation(Player& player, const sf::Clock & clock, const sf::Int32 & offset, const bool& lerp_mode);
	void quadraticInterpolation(Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& lerp_mode);

	// MATCH 2 - create a packet to be send
	sf::Packet groupIntoPacket(const PlayerMessage& player_message_send);
	// MATCH 1 - receive a message from a packet
	void receivePacket(sf::Packet& packet_receive);

	// send packet
	void sendPacket(const Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& debug_mode);
	void checkForIncomingPackets(const bool& debug_mode);

};

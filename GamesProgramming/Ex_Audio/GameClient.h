#pragma once
#include <deque>
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
	float lerp(float start, float end, float time);
	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float& time);

	sf::Int32 getCurrentTime(const sf::Clock& clock, const sf::Int32& offset);
	void addMessage(PlayerMessage& player_message, const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	unsigned int num_messages;
	std::deque<PlayerMessage> local_positions;
	void keepTrackOfLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfNetworkPositions(const PlayerMessage& player_message_receive);
	std::deque<PlayerMessage> network_positions;

	sf::Vector2f predictLinearLocalPath(const sf::Clock& clock, const sf::Int32& offset);
	sf::Vector2f predictLinearNetworkPath(const sf::Clock& clock, const sf::Int32& offset);

	sf::Vector2f predictQuadraticLocalPath(const sf::Clock & clock, const sf::Int32 & offset);
	sf::Vector2f predictQuadraticNetworkPath(const sf::Clock & clock, const sf::Int32 & offset);

	void linearInterpolation(Player& player, const sf::Clock & clock, const sf::Int32 & offset, const bool& lerp_mode);
	void quadraticInterpolation(Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& lerp_mode);

	void sendPacket(const Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& debug_mode);
	void checkForIncomingPackets(const bool& debug_mode);
};

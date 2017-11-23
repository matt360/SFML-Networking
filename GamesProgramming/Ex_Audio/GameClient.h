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
	float lerp(float start, float end, float time);
	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float& time);

	sf::Int32 getCurrentTime(const sf::Clock& clock, const sf::Int32& offset);
	void addMessage(PlayerMessage& player_message, const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	unsigned int num_messages;

	std::queue<PlayerMessage> linear_local_positions;
	void keepTrackOfLinearLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfLinearNetworkPositions(const PlayerMessage& player_message_receive);
	std::queue<PlayerMessage> linear_network_positions;
	sf::Vector2f predictLinearLocalPath(const sf::Clock& clock, const sf::Int32& offset);
	sf::Vector2f predictLinearNetworkPath(const sf::Clock& clock, const sf::Int32& offset);

	std::deque<PlayerMessage> quadratic_local_positions;
	void keepTrackOfQuadraticLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfQuadraticLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfQuadraticNetworkPositions(const PlayerMessage& player_message_receive);
	std::deque<PlayerMessage> quadratic_network_positions;
	sf::Vector2f predictQuadraticLocalPath(const sf::Clock & clock, const sf::Int32 & offset);
	sf::Vector2f predictQuadraticNetworkPath(const sf::Clock & clock, const sf::Int32 & offset);
	void linearInterpolation(Player& player, const sf::Clock & clock, const sf::Int32 & offset, const bool& lerp_mode);
	void quadraticInterpolation(Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& lerp_mode);

	void sendPacket(const Player& player, const sf::Clock& clock, const sf::Int32& offset, const bool& debug_mode);
	void checkForIncomingPackets(const bool& debug_mode);
};

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

	float lerp(float start, float end, float time);
	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float& time);
	// debug
	bool lerp_mode;

	sf::Int32 getCurrentTime(const sf::Clock& clock, const sf::Int32& offset);
	void addMessage(PlayerMessage& player_message, const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	unsigned int num_messages = 1; // 1 - for linear interpolation, 2 - for quadratic interpolation
	std::deque<PlayerMessage> local_positions;
	void keepTrackOfLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfNetworkPositions(const PlayerMessage& player_message_receive);
	std::deque<PlayerMessage> network_positions;

	sf::Vector2f predict_local_path(const sf::Clock& clock, const sf::Int32& offset);
	sf::Vector2f predict_network_path(const sf::Clock& clock, const sf::Int32& offset);

	void linearInterpolation(Player& player, const sf::Clock & clock, const sf::Int32 & offset);
	void QuadraticInterpolation(Player& player, const sf::Clock& clock, const sf::Int32& offset);

	void sendPacket(const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	void checkForIncomingPackets();

	bool debug_mode;
	bool debug_message;
};

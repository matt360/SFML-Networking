#pragma once
#include <SFML/Network.hpp>
#include "Player.h"
#include "Messages.h"
#include "Prediction.h"
#include <queue>
#include <deque>

class QuadraticPrediction : public Prediction
{
public:
	QuadraticPrediction();
	~QuadraticPrediction();

protected:
	// number of messages to keep track of
	const unsigned int quadratic_message_number = 3;
	std::deque<PlayerMessage> quadratic_local_positions;
	void keepTrackOfQuadraticLocalPositoins(const Sprite& player, const sf::Int32& tm);
	void keepTrackOfQuadraticLocalPositoins(sf::Vector2f& vec, const sf::Int32& tm);
	void keepTrackOfQuadraticNetworkPositions(const PlayerMessage& player_message_receive);
	std::deque<PlayerMessage> quadratic_network_positions;
	sf::Vector2f predictQuadraticLocalPath(const sf::Int32& tm);
	sf::Vector2f predictQuadraticNetworkPath(const sf::Int32& tm);

	void quadraticInterpolation(Sprite& player, const sf::Int32& tm, const bool& lerp_mode);
};


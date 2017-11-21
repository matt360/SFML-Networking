#pragma once

/** A position update message. */
struct PlayerMessage {
	/** The object number of this Player within the game world. */
	int id;

	/** The coordinates of this Player within the game world. */
	sf::Vector2f position;

	/** The time at which this message was sent.
	    (Not the time at which it was received!) */
	float time;
};

static sf::Packet& operator <<(sf::Packet& packet, const PlayerMessage& player_message)
{
	return packet << player_message.id << player_message.position.x << player_message.position.y << player_message.time;
}

static sf::Packet& operator >> (sf::Packet& packet, PlayerMessage& player_message)
{
	return packet >> player_message.id >> player_message.position.x >> player_message.position.y >> player_message.time;
}

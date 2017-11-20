#pragma once

/** A position update message. */
struct PlayerMessage {
	/** The object number of this Player within the game world. */
	int id;

	/** The coordinates of this Player within the game world. */
	float x, y;

	/** The time at which this message was sent.
	    (Not the time at which it was received!) */
	float time;
};

static sf::Packet& operator <<(sf::Packet& packet, const PlayerMessage& player_message)
{
	return packet << player_message.id << player_message.x << player_message.y << player_message.time;
}

static sf::Packet& operator >> (sf::Packet& packet, PlayerMessage& player_message)
{
	return packet >> player_message.id >> player_message.x >> player_message.y >> player_message.time;
}

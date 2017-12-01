#pragma once

/** A position update message. */
struct Message {
	/** The object number of this Player within the game world. */
	int id;

	/** The coordinates of this Player within the game world. */
	sf::Vector2f position;

	/** The coordinates of this Enemy within the game world. */
	sf::Vector2f enemy_position;

	/** The time at which this message was sent.
	    (Not the time at which it was received!) */
	float time;
};

static sf::Packet& operator <<(sf::Packet& packet, const Message& player_message)
{
	return packet << player_message.id << player_message.position.x << player_message.position.y << player_message.time;
}

static sf::Packet& operator >> (sf::Packet& packet, Message& player_message)
{
	return packet >> player_message.id >> player_message.position.x >> player_message.position.y >> player_message.time;
}

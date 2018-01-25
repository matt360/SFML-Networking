#pragma once

/** A player_position update message. */
struct Message {
	/** The object number of this Player within the game world. */
	int id;

	/** The coordinates of this Player within the game world. */
	sf::Vector2f player_position;

	/** The coordinates of this Enemy within the game world. */
	sf::Vector2f enemy_position;

	/** The time at which this message was sent.
	    (Not the time at which it was received!) */
	float time;
};

static sf::Packet& operator <<(sf::Packet& packet, const Message& message)
{
	return packet << message.id << 
		message.player_position.x << message.player_position.y << 
		message.enemy_position.x << message.enemy_position.y <<
		message.time;
}

static sf::Packet& operator >> (sf::Packet& packet, Message& message)
{
	return packet >> message.id >> 
		message.player_position.x >> message.player_position.y >>
		message.enemy_position.x >> message.enemy_position.y >> 
		message.time;
}

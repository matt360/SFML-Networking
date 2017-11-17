#include "MyPacket.h"

MyPacket::MyPacket()
{
}

MyPacket::~MyPacket()
{
}

sf::Packet& operator <<(sf::Packet& packet, const PlayerMessage& player_message)
{
	return packet << player_message.id << player_message.x << player_message.y << player_message.time;
}

sf::Packet& operator >>(sf::Packet& packet, PlayerMessage& player_message)
{
	return packet >> player_message.id >> player_message.x >> player_message.y >> player_message.time;
}
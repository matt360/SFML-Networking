#include "MyPacket.h"

MyPacket::MyPacket()
{
}

MyPacket::~MyPacket()
{
}

void MyPacket::addMessage(const PlayerMessage& player_message)
{
	this->operator<<(player_message.id).operator<<(player_message.x).operator<<(player_message.y).operator<<(player_message.time);
}

//sf::Packet& MyPacket::operator >> (PlayerMessage& player_message)
//{
//	return sf::Packet::operator>>(player_message.id) >> player_message.x >> player_message.y >> player_message.time;
//}
//
//sf::Packet& operator <<(sf::Packet& packet, const PlayerMessage& player_message)
//{
//	return packet << player_message.id << player_message.x << player_message.y << player_message.time;
//}
//
//sf::Packet& operator >>(sf::Packet& packet, PlayerMessage& player_message)
//{
//	return packet >> player_message.id >> player_message.x >> player_message.y >> player_message.time;
//}
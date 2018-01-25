#include "GameState.h"

GameState::GameState()
{
	// player
	player_texture.loadFromFile("gfx/MushroomTrans.png");
	
	// enemy
	enemy_texture.loadFromFile("gfx/EnemyMushroomTrans.png");

	frame = 0;
}

GameState::~GameState()
{

}

// check AABB
bool GameState::checkCollision(Sprite* s1, Sprite* s2)
{
	if (s1->getAABB().left + s1->getAABB().width < s2->getAABB().left)
		return false;
	if (s1->getAABB().left > s2->getAABB().left + s2->getAABB().width)
		return false;
	if (s1->getAABB().top + s1->getAABB().height < s2->getAABB().top)
		return false;
	if (s1->getAABB().top > s2->getAABB().top + s2->getAABB().height)
		return false;

	return true;
}

// check Sphere bounding collision
bool GameState::checkSphereBounding(Sprite* s1, Sprite* s2)
{
	// half width, height, give us the centre of the shape
	if (pow(s2->getPosition().x - s1->getPosition().x, 2) + pow(s2->getPosition().y - s1->getPosition().y, 2) < pow((s2->getSize().x / 2) + (s1->getSize().x / 2), 2))
	{
		return true;
	}
	return false;
}

void GameState::displayMessage(const Message player_message)
{
	// The message from the client
	std::cout << "\n\nSERVER: Message received from the client:";
	// Data extracted successfully...
	std::cout << "\nSERVER: ID: " << player_message.id
		<< "\nSERVER: Player x: " << player_message.player_position.x
		<< "\nSERVER: Player y: " << player_message.player_position.y
		<< "\nSERVER: Time: " << player_message.time;
}

void GameState::displayMessage(const Message player_message, const sf::IpAddress sender, const unsigned short sender_port)
{
	// The message from the client
	std::cout << "\n\nSERVER: Message received from the client:";
	// Data extracted successfully...
	std::cout << "\nSERVER: ID: " << player_message.id
		<< "\nSERVER: Player x: " << player_message.player_position.x
		<< "\nSERVER: Player y: " << player_message.player_position.y
		<< "\nSERVER: Time: " << player_message.time;
	std::cout << "\nSERVER: client's IP: " << sender;
	std::cout << "\nSERVER: client's port: " << sender_port;
}
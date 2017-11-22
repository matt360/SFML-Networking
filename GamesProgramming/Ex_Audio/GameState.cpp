#include "GameState.h"

GameState::GameState()
{
	fps = 0;
}

GameState::~GameState()
{

}

//void GameState::render()
//{
//	beginDraw();
//
//	level.render(window);
//	window->draw(player);
//	//window->draw(text);
//
//	endDraw();
//}

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

void GameState::displayMessage(const PlayerMessage player_message)
{
	// The message from the client
	std::cout << "\n\nSERVER: Message received from the client:";
	// Data extracted successfully...
	std::cout << "\nSERVER: ID: " << player_message.id
		<< "\nSERVER: Player x: " << player_message.position.x
		<< "\nSERVER: Player y: " << player_message.position.y
		<< "\nSERVER: Time: " << player_message.time;
}

void GameState::displayMessage(const PlayerMessage player_message, const sf::IpAddress sender, const unsigned short sender_port)
{
	// The message from the client
	std::cout << "\n\nSERVER: Message received from the client:";
	// Data extracted successfully...
	std::cout << "\nSERVER: ID: " << player_message.id
		<< "\nSERVER: Player x: " << player_message.position.x
		<< "\nSERVER: Player y: " << player_message.position.y
		<< "\nSERVER: Time: " << player_message.time;
	std::cout << "\nSERVER: client's IP: " << sender;
	std::cout << "\nSERVER: client's port: " << sender_port;
}

//void GameState::call_once_set_window(sf::RenderWindow& window)
//{
//	std::call_once(set_window_flag, [&]() {
//		window.setPosition(sf::Vector2i(400, 800));
//	}
//	);
//}

void GameState::call_once_set_window(sf::Vector2i vec)
{
	std::call_once(set_window_flag, [&]() {
		// client window->setPosition(sf::Vector2i(1200, 1000));
		window->setPosition(vec);
		// server window->setPosition(sf::Vector2i(1200, 200));
	}
	);
}
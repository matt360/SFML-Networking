#include "GameState.h"

GameState::GameState()
{

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



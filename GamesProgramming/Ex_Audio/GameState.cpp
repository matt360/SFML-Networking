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
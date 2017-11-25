#include "NetworkClientState.h"

NetworkClientState::NetworkClientState(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	send_packet = true;
	//established_connection = false;

	ready = false;
	server = false;
	client = false;
	debug_mode = false;
	debug_message = false;

	// Network text
	font.loadFromFile("font/advanced_pixel-7.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Press 'S' to be a server\n\nPress 'C' to be a client\n\nPress Enter to Play");
}

NetworkClientState::~NetworkClientState()
{
}

void NetworkClientState::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

void NetworkClientState::handleInput()
{
	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		// player ready to play
		ready = true;
	}

	// toggle debug mode to display socket messages
	if (input->isKeyDown(sf::Keyboard::D))
	{
		input->setKeyUp(sf::Keyboard::D);
		debug_mode = !debug_mode;
	}
	// toggle debug messages to display messages
	if (input->isKeyDown(sf::Keyboard::M))
	{
		input->setKeyUp(sf::Keyboard::M);
		debug_message = !debug_message;
	}
}

// error handling before connecting
// error while the game is running
// states
// what I want to send
// anything that happens to the client shouldn't affect the server player
// anything that happens to the server the client must handle accordingly - if the server is dead - try to reconnect for a ceratin time - take to the network state

void NetworkClientState::update()
{
	text.setString("\n\nYou're the client\n\nConnecting to the server...\n\nPress Enter to Play");

	if (!established_connection)
	{
		establishConnectionWithServer(debug_mode);
		std::cout << "function call: establishConnectionWithServer()\n";
		std::cout << "function call: getCurrentTime(): " << getCurrentTime() << "\n";
	}

	std::cout << "function call: getCurrentTime(): " << getCurrentTime() << "\n";

	if (ready && established_connection)
	{
		game_state = GameStateEnum::GAME_CLIENT;
	}
}
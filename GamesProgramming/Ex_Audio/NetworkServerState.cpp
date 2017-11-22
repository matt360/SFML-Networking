#include "NetworkServerState.h"

NetworkServerState::NetworkServerState(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	established_connection = false;
	
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

NetworkServerState::~NetworkServerState()
{
}

void NetworkServerState::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

void NetworkServerState::handleInput()
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

// anything that happens to the client shouldn't affect the server player
// anything that happens to the server the client must handle accordingly - if the server is dead - try to reconnect for a ceratin time - take to the network state

void NetworkServerState::update()
{	
	text.setString("\n\nYou're the server\n\nWaiting for the client...\n\nPress Enter to Play");

	establishConnectionWithClient();

	sf::Int32 server_time = clock.getElapsedTime().asMilliseconds();
	std::cout << "server_time: " << server_time << "\n";

	if (ready && established_connection)
	{
		game_state = GameStateEnum::GAME_SERVER;
	}
}
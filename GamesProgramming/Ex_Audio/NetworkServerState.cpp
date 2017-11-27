#include "NetworkServerState.h"
#include <sstream>

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
		// player ready to play only when at least one client has been connected
		if (established_connection) 
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
	// the string buffer to convert numbers to a string
	std::ostringstream ss;

	// Put the text to display into the string buffer
	if (established_connection)
		ss << "\n\nYou're the server\n\nEstablished connection\n\nPress Enter to Play";
	else
		ss << "\n\nYou're the server\n\nWaiting for the client...\n\nPress Enter to Play";

	// display text
	text.setString(ss.str());

	if (debug_mode) std::cout << "Established connection:" << established_connection << "\n";

	if (!established_connection)
	{
		establishConnectionWithClient(debug_mode);
	}
	setReady(ready);

	sf::Int32 server_time = clock.getElapsedTime().asMilliseconds();
	if (debug_message) std::cout << "server_time: " << server_time << "\n";

	//if (ready && established_connection && all_clients_connected)
	if (ready && established_connection)
	{
		game_state = GameStateEnum::GAME_SERVER;
	}
}
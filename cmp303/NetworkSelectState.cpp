#include "NetworkSelectState.h"

NetworkSelectState::NetworkSelectState(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	ready = false;
	server = false;
	client = false;
	debug_mode = true;
	debug_message = true;

	// NetworkSelectState text
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Press 'S' to be the server\n\nPress 'C' to be the client\n\n");
}

NetworkSelectState::~NetworkSelectState()
{
}

void NetworkSelectState::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

void NetworkSelectState::handleInput()
{
	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		// player ready to play
		ready = true;
	}	

	// Client or server ?
	// toggle being the server
	if (input->isKeyDown(sf::Keyboard::S))
	{
		input->setKeyUp(sf::Keyboard::S);
		// set readiness for being the server
		server = true;
		// setting client to false let's us change the decision and to become the cleint
		client = false;
		network_state = NetworkStateEnum::SERVER;
	}
	// toggle being the client
	if (input->isKeyDown(sf::Keyboard::C))
	{
		input->setKeyUp(sf::Keyboard::C);
		// set readiness for being the client
		client = true;
		// setting server to false let's us change the decision and become the server
		server = false;
		network_state = NetworkStateEnum::CLIENT;
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

void NetworkSelectState::update()
{
	// Client or server ?
	if (server)
	{
		// create server socket
		createServerSocket();
		// for displaying text
		text.setString(
			"You're the Server\n\n"
			"Press Enter to establish the connection with\n\n"
			"The Client\n\n"
			"Press 'S' to be the server\n\n"
			"Press 'C' to be the client\n\n");

		window->setTitle("Server");

		server = false;
	}
	if (client)
	{
		// create client socket
		createClientSocket();
		// for displaying text
		text.setString(
			"You're the Client\n\n"
			"Press Enter to establish the connection with\n\n"
			"The Server\n\n"
			"Press 'S' to be the server\n\n"
			"Press 'C' to be the client\n\n");

		window->setTitle("Client");

		client = false;
	}

	if (ready) 
	{
		switch (network_state)
		{
		case (NetworkStateEnum::SERVER):
			game_state = GameStateEnum::GAME_SERVER;
			break;
		case (NetworkStateEnum::CLIENT):
			game_state = GameStateEnum::GAME_CLIENT;
			break;
		default:
			ready = false;
			break;
		}
	}
}
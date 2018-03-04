////////////////////////////////////////////////////////////////////////////////////////////
// Application based on Paul Robertson's (p.robertson@abertay.ac.uk) CMP105 SFML Framework
// with changes made by Mateusz Zaremba (1502616@abertayac.uk):
// Audio: no changes
// App: new: Enemy class (based on the previous Player class); changes: Player class.
// System: Main.cpp game state machine changed a lot
//
// Developed by Mateusz Zaremba and based on Adam Sampson's (a.sampson@abertay.ac.uk) lab exercises with
// his help and insight: 
// Network: new
// Game: new
// App: added: MenuState, NetworkSelect, NetworkSelectState

// Application demonstrates: 
// • a simulated world containing two moveable objects, of which one is controllable by the user; 
// • network communication between machines (running multiple copies of the same program);
// • synchronisation of objects' positions between the machines, so that all machines are capable of showing a consistent view of the game world; 
// • using appropriate prediction and interpolation techniques to maintain smoothlysynchronised movement even 
//   when the network connection is affected by latency. 
////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include "MenuState.h"
#include "NetworkSelectState.h"
#include "GameClientState.h"
#include "GameServerState.h"

void main(int argc, char** argv[])
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "SFML-Networking"); // , sf::Style::None);
	// Set framerate to 60FPS
	window.setFramerateLimit(60);
	//window.setPosition(sf::Vector2i(250, 250));
	//window.setSize(sf::Vector2u(400, 400));
	////////////////////////////////

	bool pause = false;
	bool debug_mode = false;
	Input input;

	// initial game state is set in the State class
	State* state = new Menu(&window, &input);
	State* network = new NetworkSelectState(&window, &input);
	State* game_server = new GameServerState(&window, &input);
	State* game_client = new GameClientState(&window, &input);
	
	//direction dir = direction::left;

	//sf::Texture mushroomTexture;
	//mushroomTexture.loadFromFile("Mushroom.png");
	////mushroomTexture.loadFromFile("sfml-logo-small.png");
	//sf::Sprite mushroom(mushroomTexture);
	//sf::Vector2u size = mushroomTexture.getSize();
	//mushroom.setOrigin(size.x / 2, size.y / 2);
	//mushroom.setPosition(50, 50);
	//sf::Vector2f increment(0.1f, 0.1f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::Resized:
					window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
					break;
				case sf::Event::KeyPressed:
					// update input class
					input.setKeyDown(event.key.code);
					break;
				case sf::Event::KeyReleased:
					//update input class
					input.setKeyUp(event.key.code);
					break;
				case sf::Event::MouseMoved:
					//update input class
					input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						//update input class
						input.setMouseLeftDown(true);
					}
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						//update input class
						input.setMouseLeftDown(false);
					}
					break;
				default:
					// don't handle other events
					break;
			}
		}

		// if escape is pressed
		if (input.isKeyDown(sf::Keyboard::Escape))
		{
			input.setKeyUp(sf::Keyboard::Escape);
			window.close();
		}

		// game loop
		// Update/Render object based on current game state

		// abstract state class to be inherited by the state classes
		// put rendering into state classes
		// put networing into networking classes

		switch (state->getGameState())
		{
		case (GameStateEnum::MENU) :
			// the State class has an instance of the GameStateEnum and initializes it
			// to the MENU state so there is no state assignment
			break;

		case (GameStateEnum::NETWORK) :
			state = network;
			break;

		case(GameStateEnum::GAME_SERVER):
			state = game_server;
			break;

		case(GameStateEnum::GAME_CLIENT):
			state = game_client;
			break;
		}

		// Play the game
		state->handleInput();
		state->update();
		state->render();
	}
}

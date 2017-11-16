#include <iostream>
#include <cstdlib>
#include "GameClient.h"
#include "GameServer.h"
#include "Menu.h"
#include "Network.h"
#include "States.h"

void main(int argc, char** argv[])
{
	// NETWORK /////////////////////
	// global socket
	static sf::UdpSocket socket;
	// Choose an arbitrary port for opening sockets
	unsigned short port = 50001; 
	// IP adrress to connect to
	sf::IpAddress ip_address = "127.1.0";
	////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML-Networking"); // , sf::Style::None);
	// Set framerate to 60FPS
	window.setFramerateLimit(60);
	//window.setPosition(sf::Vector2i(250, 250));
	//window.setSize(sf::Vector2u(400, 400));
	////////////////////////////////
	GameState state = GameState::GAME_SERVER;
	
	bool pause = false;
	Input input;
	Menu menu(&window, &input, &state);
	Network network(&window, &input, &state, &socket, &ip_address, &port);
	GameClient game_client(&window, &input, &state, &socket, &ip_address, &port);
	GameServer game_server(&window, &input, &state, &socket, &ip_address, &port);
	
	//direction dir = direction::left;
	
	// For Delta Time
	sf::Clock clock;
	float deltaTime;
	
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

		/*if (input.isKeyDown(sf::Keyboard::Pause))
		{
			input.setKeyUp(sf::Keyboard::Pause);
			pause = !pause;

			if (pause)
			{
				state = GameState::PAUSE;
			}
			else
			{
				switch (state)
				{
				case (GameState::GAME_CLIENT):
					state = GameState::GAME_CLIENT;
					break;
				case (GameState::GAME_SERVER):
					state = GameState::GAME_SERVER;
					break;
				}
			}
		}*/

		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();
		
		// game loop
		// Update/Render object based on current game state
		switch (state)
		{
		case (GameState::MENU) :


			menu.handleInput(deltaTime);
			menu.update(deltaTime);
			menu.render();
			break;

		case (GameState::NETWORK) :
			network.handleInput(deltaTime);
			// create socket and 
			network.update(deltaTime);
			network.render();
			break;

		case(GameState::GAME_CLIENT):
			game_client.handleInput(deltaTime);
			// do a non blocking receive - put socket into non blocking mode when it's being created
			// queue of messages to send, put message into the queue when ready to send, non-blocking send
			//socket(); 
			game_client.update(deltaTime);
			game_client.render();
			break;

		case(GameState::GAME_SERVER):
			game_server.handleInput(deltaTime);
			// do a non blocking receive - put socket into non blocking mode when it's being created
			// queue of messages to send, put message into the queue when ready to send, non-blocking send
			//socket(); 
			game_server.update(deltaTime);
			game_server.render();
			break;

		case(GameState::PAUSE) :
			switch (state)
			{
			case (GameState::GAME_CLIENT):
				game_client.render();
				break;
			case (GameState::GAME_SERVER):
				game_server.render();
				break;
			}
			break;
			
		case(GameState::CREDITS) :
			//...
			break;
		}	
	}
}
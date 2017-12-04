#include "GameServerState.h"
#include <sstream>

GameServerState::GameServerState(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	debug_mode = false;
	
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(10, 0);
	text.setString("");

	// player
	player.setSize(sf::Vector2f(32, 32));
	player.setTexture(&player_texture);
	player.setPosition(250, 250);
	player.setInput(input);
	player.setAlive(true);
	//player.setVelocity(5, 5);

	// enemy
	enemy.setSize(sf::Vector2f(32, 32));
	enemy.setTexture(&enemy_texture);
	enemy.setPosition(window->getSize().x - 100, window->getSize().y - 100);
	//enemy.setVelocity(3, 3);

	//window->setMouseCursorVisible(false);
}

GameServerState::~GameServerState() {}

void GameServerState::handleInput()
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.

	// toggle debug mode to display socket messages
	if (input->isKeyDown(sf::Keyboard::D))
	{
		input->setKeyUp(sf::Keyboard::D);
		debug_mode = !debug_mode;
	}

	// toggle lerp mode
	if (input->isKeyDown(sf::Keyboard::L))
	{
		input->setKeyUp(sf::Keyboard::L);
		lerp_mode = !lerp_mode;
	}

	if (input->isKeyDown(sf::Keyboard::P))
	{
		input->setKeyUp(sf::Keyboard::P);
		linear_prediction = !linear_prediction;
		quadratic_prediction = !quadratic_prediction;
	}

	if (input->isKeyDown(sf::Keyboard::H))
	{
		input->setKeyUp(sf::Keyboard::H);
		hertz < 5 ? hertz++ : hertz = 0;
	}

	if (input->isKeyDown(sf::Keyboard::C))
	{
		input->setKeyUp(sf::Keyboard::C);
		established_connection = false;
	}

	if (input->isKeyDown(sf::Keyboard::R))
	{
		input->setKeyUp(sf::Keyboard::R);
		player.setAlive(true);
	}
}

void GameServerState::render()
{
	beginDraw();

	//level.render(window);
	window->draw(player);
	window->draw(enemy);
	window->draw(text);

	endDraw();
}

void GameServerState::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void GameServerState::endDraw()
{
	window->display();
}

void GameServerState::setHertz()
{
	switch (hertz)
	{
	case 0:
		// 10 Hz
		hertz_rate = 6;
		hertz_string = "10";
		break;

	case 1:
		// 12Hz
		hertz_rate = 5;
		hertz_string = "12";
		break;

	case 2:
		// 15Hz
		hertz_rate = 4;
		hertz_string = "15";
		break;

	case 3:
		// 20Hz
		hertz_rate = 3;
		hertz_string = "20";
		break;

	case 4:
		// 30Hz
		hertz_rate = 2;
		hertz_string = "30";
		break;

	case 5:
		// 60 Hz
		hertz_rate = 1;
		hertz_string = "60";
		break;
	}
}

void GameServerState::displayText()
{
	//string buffer to convert numbers to string
	std::ostringstream ss;

	ss << "LERP MODE: " << lerp_mode << " LINEAR PREDICTION: " << linear_prediction << " QUADRATIC PREDICTION: " << quadratic_prediction << "\n"
		<< "IP: " << Network::ip_address.getLocalAddress() << " PORT: " << Network::socket.getLocalPort() << " SEND MSG AT: " << hertz_string << "Hz" << " CLOCK: " << getCurrentTime(clock, offset) << "\n"
		<< "NUMBER OF CLIENTS: " << addresses.size() << "\n"
		<< "PLAYER ALIVE: " << player.isAlive();

	// display text
	text.setString(ss.str());
}

void GameServerState::keepSpriteWithinWindow(Sprite& sprite)
{
	// keep the sprite within window's x
	if (sprite.getPosition().x > window->getSize().x - sprite.getSize().x) sprite.setVelocity(-15, 0);
	if (sprite.getPosition().x < 0) sprite.setVelocity(15, 0);
	// keep the sprite within window's y
	if (sprite.getPosition().y > window->getSize().y - sprite.getSize().y) sprite.setVelocity(0, -15);
	if (sprite.getPosition().y < 0) sprite.setVelocity(0, 15);
}

void GameServerState::checkForClient()
{
}

// SERVER //
void GameServerState::syncClocksWithClient(const bool& debug_mode)
{
	// Wait for a message...
	// Receive the packet at the other end
	// TODO proper address and port
	sf::Packet packet_receive;
	switch (socket.receive(packet_receive, Network::ip_address, Network::port))
	{
	case sf::Socket::Partial:
		// clear the packet if only part of the data was received
		packet_receive.clear();
		break;

	case sf::Socket::Done:
		// TODO keep track of the new sockets GET THE ADDRESS OF THE NEW CLIENT HERE
		// check if the address is new if so...
		// The insertion only takes place if no other element in the container is equivalent to the one being emplaced (elements in a set container are unique).
		//addresses.emplace(Network::port);
		// check if the address is new if so...
		if (addresses.find(Network::port) == addresses.end()) {
			// ...this is an address we've not seen before.
			latestID++;
			printf("New address, giving it ID %d\n", latestID);
			// create a pair
			addresses[latestID] = Network::port;
			new_connection = true;
		}
		// Received a packet.
		if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";
		return;

	case sf::Socket::Disconnected:
		if (debug_mode) std::cout << "CLIENT: Disconnected\n";
		return;

	case sf::Socket::Error:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
		return;
	}

	///////////////////////////////////////////////////////////////////////////
	// RECEIVE (from the client) MUST MATCH packet_send in the NetworkClient //
	///////////////////////////////////////////////////////////////////////////
	// Extract the variables contained in the packet
	// Deal with the messages from the packet
	// WITHOUT THIS THE CLIENT WILL NOT RESPOND AND ITS SOCKET WILL RETURN NOTREADY
	// AND THE SERVERS SOCKET WILL RETURN ERROR
	bool hello;
	if (packet_receive >> hello)
	{
		// The message from the client
		established_connection = hello;
	}

	// if tried to connect
	established_connection = true;
	/////////////////////////////////////////////////////////////////////////
	// SEND (to the client) MUST MATCH packet_receive in the NetworkClient //
	/////////////////////////////////////////////////////////////////////////
	if (new_connection)
	{
		// Message to send
		sf::Packet send_packet;
		// Group the variables to send into a packet
		sf::Int32 server_time = clock.getElapsedTime().asMilliseconds();
		send_packet << server_time << established_connection;

		// SEND THE SERVER'S TIME TO SYNC THE CLIENT'S AND THE SERVER'C CLOCKS
		// Send it over the network
		switch (socket.send(send_packet, Network::ip_address, addresses.at(latestID)))
		{
		case sf::Socket::Partial:
			// 
			/*
			https://www.sfml-dev.org/tutorials/2.4/network-socket.php
			if only a part of the data was sent in the call, the return status will be sf::Socket::Partial to indicate a partial send.
			If sf::Socket::Partial is returned, you must make sure to handle the partial send properly or else data corruption will occur.
			When sending raw data, you must reattempt sending the raw data at the byte offset where the previous send call stopped.
			*/
			while (socket.send(send_packet, Network::ip_address, Network::port) != sf::Socket::Done) {}
			break;

		case sf::Socket::Done:
			// Received a packet.
			new_connection = false;
			if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
			break;

		case sf::Socket::NotReady:
			// No more data to receive (yet).
			if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";
			return;

		case sf::Socket::Disconnected:
			if (debug_mode) std::cout << "CLIENT: Disconnected\n";
			return;

		case sf::Socket::Error:
			// Something went wrong.
			if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
			return;

		default:
			// Something went wrong.
			if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
			return;
		}
	}

	/// Extract the variables contained in the packet
}

void GameServerState::update()
{
	// reset the counter because the game is locked at 60fps
	if (frame > 60) frame = 0;

	setHertz();

	// display text
	displayText();

	checkForClient();

	// CONNECT A NEW CLIENT
	// WITHOUT established_connection THE CLIENT WILL NOT RESPOND AND ITS SOCKET WILL RETURN NOTREADY AND THE SERVERS SOCKET WILL RETURN ERROR
	if (!established_connection)
		syncClocksWithClient(debug_mode);

	player.update();
	// keep the player within the window
	keepSpriteWithinWindow(player);
	// keep the enemy within the window
	keepSpriteWithinWindow(enemy);

	enemy.setPosition(lerp(enemy.getPosition(), player.getPosition(), 0.01));

	if (checkSphereBounding(&player, &enemy))
		player.collisionRespone(&enemy);

	// server should probably keep listening and sending all the time
	// send messages at 15Hz
	if (frame % hertz_rate == 0)
	{
		for (auto& pair : addresses)
			sendMessageToClient(player, enemy, clock, pair.second, debug_mode);
	}

	// keep track of frames
	frame++;
}


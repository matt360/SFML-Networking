#include "GameClientState.h"
#include <sstream>

GameClientState::GameClientState(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	lerp_mode = true;
	debug_mode = false;
	debug_message = false;

	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(10, 0);
	text.setString("");

	player_texture.loadFromFile("gfx/MushroomTrans.png");

	player.setSize(sf::Vector2f(32, 32));
	player.setTexture(&player_texture);
	sf::Vector2f initial_player_position(5.0f, 5.0f);
	player.setPosition(initial_player_position);

	enemy.setSize(sf::Vector2f(32, 32));
	enemy.setTexture(&player_texture);
	enemy.setPosition(window->getSize().x - 100, window->getSize().y - 100);

	//window->setMouseCursorVisible(false);

	level.loadTexture("gfx/marioTiles.png");
	Tile tile;
	std::vector<Tile> tiles;
	//187, 51
	for (int i = 0; i < 7; i++)
	{

		tile.setSize(sf::Vector2f(32, 32));
		tile.setAlive(true);
		tiles.push_back(tile);
	}

	tiles[0].setAlive(false);
	tiles[0].setTextureRect(sf::IntRect(187, 51, 16, 16));
	tiles[1].setTextureRect(sf::IntRect(0, 0, 16, 16));
	tiles[2].setTextureRect(sf::IntRect(17, 0, 16, 16));
	tiles[3].setTextureRect(sf::IntRect(34, 0, 16, 16));
	tiles[4].setTextureRect(sf::IntRect(0, 34, 16, 16));
	tiles[5].setTextureRect(sf::IntRect(17, 34, 16, 16));
	tiles[6].setTextureRect(sf::IntRect(34, 34, 16, 16));

	level.setTileSet(tiles);

	// Map dimensions
	sf::Vector2u mapSize(10, 6);

	// build map
	std::vector<int> map = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 3, 0, 0, 0, 0, 0,
		1, 2, 3, 4, 6, 1, 2, 3, 0, 0,
		4, 5, 6, 0, 0, 4, 5, 6, 0, 0
	};

	/*std::vector<int> map = {
	1, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 4, 6, 1, 3, 0, 0, 0, 0,
	0, 0, 0, 0, 4, 6, 1, 3, 0, 0,
	0, 0, 0, 0, 0, 0, 4, 6, 1, 3,
	0, 0, 0, 0, 0, 0, 0, 0, 4, 6
	};*/

	level.setTileMap(map, mapSize);
	level.setPosition(sf::Vector2f(0, 408));
	level.buildLevel();
}

GameClientState::~GameClientState() {}

void GameClientState::render()
{
	beginDraw();

	level.render(window);
	window->draw(player);
	window->draw(enemy);
	window->draw(text);

	endDraw();
}

void GameClientState::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void GameClientState::endDraw()
{
	window->display();
}

void GameClientState::handleInput()
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.
	
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

// CLIENT //
// Send a message to the server...
void GameClientState::sendPacketToServer(const bool& debug_mode)
{
	/////////////////////////////////////////////////////////////////////////////////////
	// RECEIVE (what server receives) - MUST MATCH packet_receive in the NetworkServer //
	/////////////////////////////////////////////////////////////////////////////////////
	// Group the variables to send into a packet
	sf::Packet packet_to_send;
	// Message to send
	bool hello = true;
	packet_to_send << hello;
	// Send it over the network
	switch (socket.send(packet_to_send, Network::ip_address, Network::port))
	{
	case sf::Socket::Partial:
		while (sf::Socket::Done) { socket.send(packet_to_send, Network::ip_address, Network::port); }
		break;

	case sf::Socket::Done:
		// send a packet.
		// stop timing latency
		clocks_synced = true;
		//GameClientState::ip_address = Network::ip_address;
		//GameClientState::port = Network::port;
		if (debug_mode) std::cout << "\nCLIENT: Sent one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		// allow for timing latency when the client is establishing the connection
		clocks_synced = false;
		if (debug_mode) std::cout << "\nCLIENT: Can't send now\n";
		std::cout << "send_packet is true" << "\n";
		//if (debug_mode) 
		return;

	case sf::Socket::Disconnected:
		established_connection = false;
		return;

	case sf::Socket::Error:
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: send didn't return Done\n";
		return;
	}

	/// Extract the variables contained in the packet
	//if (packet_to_send >> hello)
	//{
	//	send_packet = false;
	//	// Data extracted successfully...
	//}
}

// CLIENT //
// ...wait for the answer
void GameClientState::checkForIncomingPacketsFromServer(const bool& debug_mode)
{
	// CHECK FOR INCOMING PACKETS
	while (true)
	{
		// Try to receive the packet from the other end
		///////////////////////////////////////////////////////////////////////////////
		// SEND (What server is sending) MUST MATCH packet_send in the NetworkServer //
		///////////////////////////////////////////////////////////////////////////////
		// TODO where the address changes
		sf::Packet packet_receive;
		switch (socket.receive(packet_receive, Network::ip_address, Network::port))
		{
		case sf::Socket::Partial:
			break;

		case sf::Socket::Done:
			// Received a packet.
			if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
			//GameClientState::ip_address = Network::ip_address;
			//GameClientState::port = Network::port;
			break;

		case sf::Socket::NotReady:
			// No more data to receive (yet).
			if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";
			return;

		case sf::Socket::Disconnected:
			established_connection = false;
			return;

		case sf::Socket::Error:
			return;

		default:
			// Something went wrong.
			if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
			return;
		}

		// Extract the variables contained in the packet
		// Packets must match to what the server is sending (e.g.: server is sending string, client must expect string)
		if (packet_receive >> server_time >> established_connection)
		{
			// Data extracted successfully...
			// Deal with the messages from the packet
			std::cout << "send_packet is false" << "\n";
			// end timing latency
			end_timing_latency = clock.getElapsedTime().asMilliseconds();
			std::cout << "end_timing_latency: " << end_timing_latency << "\n";
			latency = (end_timing_latency - start_timing_latency);
			std::cout << "latency: " << latency << "\n";
			// calculate server time
			sf::Int32 client_time = clock.getElapsedTime().asMilliseconds();
			std::cout << "client_time: " << client_time << "\n";
			// server_time from the message
			offset = ((server_time + (0.5 * latency)) - client_time);
			std::cout << "offset: " << offset << "\n";

			return;
		}
	}
}

// CLIENT //
void GameClientState::establishConnectionWithServer(const bool& debug_mode)
{
	// send message to the server...
	if (!clocks_synced)
	{
		// set the lag
		// TODO is lag good here? sf::sleep(sf::milliseconds(lag));
		// start timing latency	
		start_timing_latency = clock.getElapsedTime().asMilliseconds();
		std::cout << "start_timing_latency: " << start_timing_latency << "\n";
		sendPacketToServer(debug_mode);
	}

	// ...wait for the answer
	checkForIncomingPacketsFromServer(debug_mode);
}

// keep track of player's local positions for linear and quadratic prediction
void GameClientState::keepTrackOfPlayerLocalPositions()
{
	Message player_local_message;
	player_local_message.player_position.x = player.getPosition().x;
	player_local_message.player_position.y = player.getPosition().y;
	player_local_message.time = getCurrentTime(clock, offset);
	player_linear_prediction.keepTrackOfLinearLocalPositoins(player_local_message);
	player_quadratic_prediction.keepTrackOfQuadraticLocalPositoins(player_local_message);
}

// keep track of enemy's local positions for linear and quadratic prediction
void GameClientState::keepTrackOfEnemyLocalPositions()
{
	Message enemy_local_message;
	enemy_local_message.enemy_position.x = enemy.getPosition().x;
	enemy_local_message.enemy_position.y = enemy.getPosition().y;
	enemy_local_message.time = getCurrentTime(clock, offset);
	enemy_linear_prediction.keepTrackOfLinearLocalPositoins(enemy_local_message);
	enemy_quadratic_prediction.keepTrackOfQuadraticLocalPositoins(enemy_local_message);
}

void GameClientState::playerLinearPrediction()
{
	// PLAYER LINEAR PREDICTION // start player's linear prediction only if the queue of local and network positions is full and the linear mode is on
	if (linear_prediction &&
		player_linear_prediction.network_message_history.size() == player_linear_prediction.linear_message_number &&
		player_linear_prediction.local_message_history.size() == player_linear_prediction.linear_message_number)
	{
		sf::Vector2f msg0_local_position(player_linear_prediction.local_message_history.front().player_position.x,
			player_linear_prediction.local_message_history.front().player_position.y);
		sf::Vector2f msg1_local_position(player_linear_prediction.local_message_history.back().player_position.x,
			player_linear_prediction.local_message_history.back().player_position.y);
		sf::Vector2f msg0_network_position(player_linear_prediction.network_message_history.front().player_position.x,
			player_linear_prediction.network_message_history.front().player_position.y);
		sf::Vector2f msg1_network_position(player_linear_prediction.network_message_history.back().player_position.x,
			player_linear_prediction.network_message_history.back().player_position.y);
		float msg0_time = player_linear_prediction.network_message_history.front().time;
		float msg1_time = player_linear_prediction.network_message_history.back().time;

		sf::Vector2f player_lerp_position = player_linear_prediction.linearInterpolation(
			player, // PLAYER
			msg0_local_position,
			msg1_local_position,
			msg0_network_position,
			msg1_network_position,
			msg0_time,
			msg1_time,
			getCurrentTime(clock, offset),
			lerp_mode);

		if (lerp_mode)
		{
			// add lerped to the history of the local posistions
			Message player_lerp_position_msg;
			player_lerp_position_msg.player_position.x = player_lerp_position.x;
			player_lerp_position_msg.player_position.y = player_lerp_position.y;
			player_lerp_position_msg.time = getCurrentTime(clock, offset);

			player_linear_prediction.keepTrackOfLinearLocalPositoins(player_lerp_position_msg);
		}
	}
}

void GameClientState::enemyLinearPrediciton()
{
	// ENEMY LINEAR PREDICTION // start enemny's linear prediction only if the queue of local and network positions is full and the linear mode is on
	if (linear_prediction &&
		enemy_linear_prediction.network_message_history.size() == enemy_linear_prediction.linear_message_number &&
		enemy_linear_prediction.local_message_history.size() == enemy_linear_prediction.linear_message_number)
	{
		sf::Vector2f msg0_local_position(enemy_linear_prediction.local_message_history.front().enemy_position.x,
			enemy_linear_prediction.local_message_history.front().enemy_position.y);
		sf::Vector2f msg1_local_position(enemy_linear_prediction.local_message_history.back().enemy_position.x,
			enemy_linear_prediction.local_message_history.back().enemy_position.y);
		sf::Vector2f msg0_network_position(enemy_linear_prediction.network_message_history.front().enemy_position.x,
			enemy_linear_prediction.network_message_history.front().enemy_position.y);
		sf::Vector2f msg1_network_position(enemy_linear_prediction.network_message_history.back().enemy_position.x,
			enemy_linear_prediction.network_message_history.back().enemy_position.y);
		float msg0_time = enemy_linear_prediction.network_message_history.front().time;
		float msg1_time = enemy_linear_prediction.network_message_history.back().time;


		sf::Vector2f enemy_lerp_position =
			enemy_linear_prediction.linearInterpolation(
				enemy, // ENEMY
				msg0_local_position,
				msg1_local_position,
				msg0_network_position,
				msg1_network_position,
				msg0_time,
				msg1_time,
				getCurrentTime(clock, offset),
				lerp_mode);

		if (lerp_mode)
		{
			// add lerped to the history of the local posistions
			Message enemy_lerp_position_msg;
			enemy_lerp_position_msg.enemy_position.x = enemy_lerp_position.x;
			enemy_lerp_position_msg.enemy_position.y = enemy_lerp_position.y;
			enemy_lerp_position_msg.time = getCurrentTime(clock, offset);

			enemy_linear_prediction.keepTrackOfLinearLocalPositoins(enemy_lerp_position_msg);
		}
	}
}

void GameClientState::playerQuadraticPrediction()
{
	// PLAYER QUADRATIC PREDICTION // start the quadratic prediction only if the queue of local and network positions is full and the quadratic mode is on
	if (quadratic_prediction &&
		player_quadratic_prediction.network_message_history.size() == player_quadratic_prediction.quadratic_message_number &&
		player_quadratic_prediction.local_message_history.size() == player_quadratic_prediction.quadratic_message_number)
	{
		// HISTORY OF LOCAL POSITIONS
		sf::Vector2f msg0_local_position(player_quadratic_prediction.local_message_history.at(0).player_position.x,
			player_quadratic_prediction.local_message_history.at(0).player_position.y);
		sf::Vector2f msg1_local_position(player_quadratic_prediction.local_message_history.at(1).player_position.x,
			player_quadratic_prediction.local_message_history.at(1).player_position.y);
		sf::Vector2f msg2_local_position(player_quadratic_prediction.local_message_history.at(2).player_position.x,
			player_quadratic_prediction.local_message_history.at(2).player_position.y);
		// HISTORY OF NETWORK POSITIONS
		sf::Vector2f msg0_network_position(player_quadratic_prediction.network_message_history.at(0).player_position.x,
			player_quadratic_prediction.network_message_history.at(0).player_position.y);
		sf::Vector2f msg1_network_position(player_quadratic_prediction.network_message_history.at(1).player_position.x,
			player_quadratic_prediction.network_message_history.at(1).player_position.y);
		sf::Vector2f msg2_network_position(player_quadratic_prediction.network_message_history.at(2).player_position.x,
			player_quadratic_prediction.network_message_history.at(2).player_position.y);
		// HISTORY OF TIME STAMPS
		float msg0_time = player_quadratic_prediction.local_message_history.at(0).time;
		float msg1_time = player_quadratic_prediction.local_message_history.at(1).time;
		float msg2_time = player_quadratic_prediction.local_message_history.at(2).time;

		sf::Vector2f player_lerp_position =
			player_quadratic_prediction.quadraticInterpolation(
				player, // PLAYER
				msg0_local_position,
				msg1_local_position,
				msg2_local_position,
				msg0_network_position,
				msg1_network_position,
				msg2_network_position,
				msg0_time,
				msg1_time,
				msg2_time,
				getCurrentTime(clock, offset),
				lerp_mode);

		if (lerp_mode)
		{
			// add lerped to the history of the local posistions
			Message player_lerp_position_msg;
			player_lerp_position_msg.player_position.x = player_lerp_position.x;
			player_lerp_position_msg.player_position.y = player_lerp_position.y;
			player_lerp_position_msg.time = getCurrentTime(clock, offset);

			player_linear_prediction.keepTrackOfLinearLocalPositoins(player_lerp_position_msg);
		}
	}
}

void GameClientState::enemyQuadraticPrediction()
{
	// ENEMY QUADRATIC PREDICTION // start the quadratic prediction only if the queue of local and network positions is full and the quadratic mode is on
	if (quadratic_prediction &&
		enemy_quadratic_prediction.network_message_history.size() == enemy_quadratic_prediction.quadratic_message_number &&
		enemy_quadratic_prediction.local_message_history.size() == enemy_quadratic_prediction.quadratic_message_number)
	{
		// HISTORY OF LOCAL POSITIONS
		sf::Vector2f msg0_local_position(enemy_quadratic_prediction.local_message_history.at(0).enemy_position.x,
			enemy_quadratic_prediction.local_message_history.at(0).enemy_position.y);
		sf::Vector2f msg1_local_position(enemy_quadratic_prediction.local_message_history.at(1).enemy_position.x,
			enemy_quadratic_prediction.local_message_history.at(1).enemy_position.y);
		sf::Vector2f msg2_local_position(enemy_quadratic_prediction.local_message_history.at(2).enemy_position.x,
			enemy_quadratic_prediction.local_message_history.at(2).enemy_position.y);
		// HISTORY OF NETWORK POSITIONS
		sf::Vector2f msg0_network_position(enemy_quadratic_prediction.network_message_history.at(0).enemy_position.x,
			enemy_quadratic_prediction.network_message_history.at(0).enemy_position.y);
		sf::Vector2f msg1_network_position(enemy_quadratic_prediction.network_message_history.at(1).enemy_position.x,
			enemy_quadratic_prediction.network_message_history.at(1).enemy_position.y);
		sf::Vector2f msg2_network_position(enemy_quadratic_prediction.network_message_history.at(2).enemy_position.x,
			enemy_quadratic_prediction.network_message_history.at(2).enemy_position.y);
		// HISTORY OF TIME STAMPS
		float msg0_time = enemy_quadratic_prediction.local_message_history.at(0).time;
		float msg1_time = enemy_quadratic_prediction.local_message_history.at(1).time;
		float msg2_time = enemy_quadratic_prediction.local_message_history.at(2).time;

		sf::Vector2f enemy_lerp_position =
			enemy_quadratic_prediction.quadraticInterpolation(
				enemy, // ENEMY
				msg0_local_position,
				msg1_local_position,
				msg2_local_position,
				msg0_network_position,
				msg1_network_position,
				msg2_network_position,
				msg0_time,
				msg1_time,
				msg2_time,
				getCurrentTime(clock, offset),
				lerp_mode);

		if (lerp_mode)
		{
			// add lerped to the history of the local posistions
			Message enemy_lerp_position_msg;
			enemy_lerp_position_msg.enemy_position.x = enemy_lerp_position.x;
			enemy_lerp_position_msg.enemy_position.y = enemy_lerp_position.y;
			enemy_lerp_position_msg.time = getCurrentTime(clock, offset);

			enemy_linear_prediction.keepTrackOfLinearLocalPositoins(enemy_lerp_position_msg);
		}
	}
}

void GameClientState::update()
{
	if (fps > 60) fps = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// ESTABLISH NEW CONNECTION - ADD THE CLIENT TO THE CONNECTION LIST - DO IT ONLY ONCE
	///////////////////////////////////////////////////////////////////////////////////////////////

	// the string buffer to convert numbers to a string
	std::ostringstream ss;

	// Put the text to display into the string buffer
	ss << "ESTABLISHED CONNECTION: " << established_connection << "\n"
		<< "SERVER TIME: " << server_time << " CLIENT TIME: " << start_timing_latency << " OFFSET: " << offset << "MS" << " LAG: " << lag << "MS" << "\n" // TODO remove lag
		<< "START TIMING LATENCY: " << start_timing_latency << " END TIMING LATENCY: " << end_timing_latency << " LATENCY: " << latency << "MS" << "\n"
		<< "IP: " << Network::ip_address.getLocalAddress() << " PORT: " << Network::socket.getLocalPort() << " CLOCK: " << getCurrentTime(clock, offset);

	// display text
	text.setString(ss.str());

	if (!established_connection)
	{
		establishConnectionWithServer(debug_mode);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	// keep track of player's local positions for linear and quadratic prediction
	keepTrackOfPlayerLocalPositions();
	// keep track of enemy's local positions for linear and quadratic prediction
	keepTrackOfEnemyLocalPositions();

	// send packets at 10Hz rate (at 10PFS)
	//if ((int)fps % 6 == 0)
	// send packets at 30Hz rate (at 30PFS)
	//if ((int)fps % 2 == 0)
	checkForIncomingPackets(debug_mode);

	playerLinearPrediction();
	enemyLinearPrediciton();
	playerQuadraticPrediction();
	enemyQuadraticPrediction();

	// increase fps
	fps++;
}
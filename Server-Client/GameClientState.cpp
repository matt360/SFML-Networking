#include "GameClientState.h"
#include <sstream>

GameClientState::GameClientState(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	lerp_mode = true;
	debug_mode = false;
	
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(10, 0);
	text.setString("");

	// player
	player.setSize(sf::Vector2f(32, 32));
	player.setTexture(&player_texture);
	sf::Vector2f initial_player_position(5.0f, 5.0f);
	player.setPosition(initial_player_position);

	// enemy
	enemy.setSize(sf::Vector2f(32, 32));
	enemy.setTexture(&enemy_texture);
	enemy.setPosition(800, 500);

	//window->setMouseCursorVisible(false);
}

GameClientState::~GameClientState() {}

void GameClientState::render()
{
	beginDraw();

	//level.render(window);
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
}

void GameClientState::displayText()
{
	// the string buffer to convert numbers to a string
	std::ostringstream ss;

	// Put the text to display into the string buffer
	ss << "ESTABLISHED CONNECTION: " << established_connection << "\n"
		<< "SERVER TIME: " << server_time << " CLIENT TIME: " << start_timing_latency << " OFFSET: " << offset << "MS" << " LAG: " << lag << "MS" << "\n"
		<< "START TIMING LATENCY: " << start_timing_latency << " END TIMING LATENCY: " << end_timing_latency << " LATENCY: " << latency << "MS" << "\n"
		<< "IP: " << Network::ip_address.getLocalAddress() << " PORT: " << Network::socket.getLocalPort() << " CLOCK: " << getCurrentTime(clock, offset);

	// display text
	text.setString(ss.str());
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

// CLIENT //
// Send a message to the server...
void GameClientState::sayHelloToServer(const bool& debug_mode)
{
	/////////////////////////////////////////////////////////////////////////////////////
	// RECEIVE (what server receives) - MUST MATCH packet_receive in the NetworkServer //
	/////////////////////////////////////////////////////////////////////////////////////
	// Group the variables to send into a packet
	sf::Packet send_packet;
	// Message to send
	bool hello = true;
	send_packet << hello;
	// Send it over the network
	switch (socket.send(send_packet, Network::ip_address, Network::port))
	{
	case sf::Socket::Partial:
		/*
		https://www.sfml-dev.org/tutorials/2.4/network-socket.php
		if only a part of the data was sent in the call, the return status will be sf::Socket::Partial to indicate a partial send.
		If sf::Socket::Partial is returned, you must make sure to handle the partial send properly or else data corruption will occur.
		When sending raw data, you must reattempt sending the raw data at the byte offset where the previous send call stopped.
		*/
		while (socket.send(send_packet, Network::ip_address, Network::port) != sf::Socket::Done) {}
		break;

	case sf::Socket::Done:
		// send a packet.
		// stop timing latency
		//GameClientState::ip_address = Network::ip_address;
		//GameClientState::port = Network::port;
		if (debug_mode) std::cout << "\nCLIENT: Sent one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		// allow for timing latency when the client is establishing the connection
		//clocks_synced = false;
		if (debug_mode) std::cout << "\nCLIENT: Can't send now\n";
		//if (debug_mode) 
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
void GameClientState::syncClockWithServer(const bool& debug_mode)
{
	// CHECK FOR INCOMING PACKETS
	while (true)
	{
		// Try to receive the packet from the other end
		///////////////////////////////////////////////////////////////////////////////
		// SEND (What server is sending) MUST MATCH packet_send in the NetworkServer //
		///////////////////////////////////////////////////////////////////////////////
		sf::Packet packet_receive;
		switch (socket.receive(packet_receive, Network::ip_address, Network::port))
		{
		case sf::Socket::Partial:
			// clear the packet if only part of the data was received
			packet_receive.clear();
			break;

		case sf::Socket::Done:
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

		// Extract the variables contained in the packet
		// Packets must match to what the server is sending (e.g.: server is sending string, client must expect string)
		if (packet_receive >> server_time >> established_connection)
		{
			// Data extracted successfully...
			// Deal with the messages from the packet
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
			std::cout << "clocks synced" << "\n";

			clocks_synced = true;

			return;
		}
	}
}

// CLIENT //
void GameClientState::syncClocks(const bool& debug_mode)
{
	// send message to the server...
	if (!clocks_synced)
	{
		// start timing latency	
		start_timing_latency = clock.getElapsedTime().asMilliseconds();
		std::cout << "start_timing_latency: " << start_timing_latency << "\n";
		sayHelloToServer(debug_mode);
		// ...wait for the answer
		syncClockWithServer(debug_mode);
	}

}

void GameClientState::update()
{
	// the client wants to receive the message as soon as it gets to it
	// so we don't need to keep track of the frames

	// display text
	displayText();

	// SYNC THE CLIENT'S CLOCK WITH THE SERVER'S CLOCK - DO IT ONLY ONCE!!!
	syncClocks(debug_mode);

	// keep track of player's local positions for linear and quadratic prediction
	keepTrackOfPlayerLocalPositions();
	// keep track of enemy's local positions for linear and quadratic prediction
	keepTrackOfEnemyLocalPositions();
	
	// 
	checkForIncomingPackets(debug_mode);

	playerLinearPrediction();
	enemyLinearPrediciton();
	playerQuadraticPrediction();
	enemyQuadraticPrediction();
}
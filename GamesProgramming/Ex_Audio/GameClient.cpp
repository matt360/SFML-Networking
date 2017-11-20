#include "GameClient.h"
#include "States.h"

GameClient::GameClient(sf::RenderWindow* hwnd, Input* in, GameState* st, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short *port_number)
{
	window = hwnd;
	input = in;
	socket = udp_socket;
	ip_address = ip;
	port = port_number;
	state = st;

	fps = 0;

	debug_mode = false;
	debug_message = false;

	font.loadFromFile("font/advanced_pixel-7.ttf");
	text.setFont(font);
	text.setCharacterSize(18);
	text.setString("hellow world");

	texture.loadFromFile("gfx/MushroomTrans.png");

	player.setSize(sf::Vector2f(32, 32));
	player.setTexture(&texture);
	player.setPosition(5, 5);
	player.setInput(input);
	player.setVelocity(0, 10);

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


	audioMgr.addMusic("sfx/cantina.wav", "cantina");
	audioMgr.addMusic("sfx/hyrulefield.wav", "hyrule");

	audioMgr.addSound("sfx/SMB_jump-small.wav", "jump");
	audioMgr.addSound("sfx/SMB_1-up.wav", "up");
	audioMgr.addSound("sfx/getover.wav", "getover");
	audioMgr.addSound("sfx/TP_Secret.wav", "secret");
	audioMgr.addSound("sfx/Glass_Break.wav", "glass");

	hasStarted = false;
	//audioMgr.playSoundbyName("cantina");

	//	int err = buff.loadFromFile("sfx/cantina.ogg");
	//soun.setBuffer(buff);
	//soun.play();
	//sf::Music music;
}

GameClient::~GameClient()
{

}

void GameClient::call_once_set_window(sf::RenderWindow& window)
{
	std::call_once(set_window_flag, [&]() {
		window.setPosition(sf::Vector2i(400, 800));
	}
	);
}

void GameClient::call_once_set_window()
{
	std::call_once(set_window_flag, [&]() {
		window->setPosition(sf::Vector2i(1200, 1000));
	}
	);
}

void GameClient::handleInput()
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

void GameClient::render()
{
	beginDraw();

	level.render(window);
	window->draw(player);
	//window->draw(text);

	endDraw();
}

void GameClient::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void GameClient::endDraw()
{
	window->display();
}

// check AABB
bool GameClient::checkCollision(Sprite* s1, Sprite* s2)
{
	if (s1->getAABB().left + s1->getAABB().width < s2->getAABB().left)
		return false;
	if (s1->getAABB().left > s2->getAABB().left + s2->getAABB().width)
		return false;
	if (s1->getAABB().top + s1->getAABB().height < s2->getAABB().top)
		return false;
	if (s1->getAABB().top > s2->getAABB().top + s2->getAABB().height)
		return false;

	return true;
}

// check Sphere bounding collision
bool GameClient::checkSphereBounding(Sprite* s1, Sprite* s2)
{
	// half width, height, give us the centre of the shape
	if (pow(s2->getPosition().x - s1->getPosition().x, 2) + pow(s2->getPosition().y - s1->getPosition().y, 2) < pow((s2->getSize().x / 2) + (s1->getSize().x / 2), 2))
	{
		return true;
	}
	return false;
}

void GameClient::displayMessage(const PlayerMessage player_message)
{
	// The message from the client
	std::cout << "\n\nSERVER: Message received from the client:";
	// Data extracted successfully...
	std::cout << "\nSERVER: ID: " << player_message.id
		<< "\nSERVER: Player x: " << player_message.x
		<< "\nSERVER: Player y: " << player_message.y
		<< "\nSERVER: Time: " << player_message.time;
}

void GameClient::displayMessage(const PlayerMessage player_message, const sf::IpAddress sender, const unsigned short sender_port)
{
	// The message from the client
	std::cout << "\n\nSERVER: Message received from the client:";
	// Data extracted successfully...
	std::cout << "\nSERVER: ID: " << player_message.id
		<< "\nSERVER: Player x: " << player_message.x
		<< "\nSERVER: Player y: " << player_message.y
		<< "\nSERVER: Time: " << player_message.time;
	std::cout << "\nSERVER: client's IP: " << sender;
	std::cout << "\nSERVER: client's port: " << sender_port;
}

void GameClient::addMessage(PlayerMessage& player_message_send)
{
	//PlayerMessage player_message_send;
	player_message_send.id = 0;
	player_message_send.x = player.getPosition().x;
	player_message_send.y = player.getPosition().y;
	
	//SYSTEMTIME system_time;
	SYSTEMTIME local_time;

	//GetSystemTime(&system_time);
	GetLocalTime(&local_time);

	//player_message_send.time = system_time.wMilliseconds;
	player_message_send.time = local_time.wMilliseconds;

	/*time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	player_message_send.time = timeinfo->tm_sec;*/
}

////////////////////////////////////////////////////////////
// Send a message to the server...
//
////////////////////////////////////////////////////////////
void GameClient::sendPacket()
{
	// message
	// RECEIVE (what server receives) - MUST MATCH packet_receive in the GameServer
	PlayerMessage player_message_send;

	// Group the variables to send into a packet
	sf::Packet packet_send;
	addMessage(player_message_send);
	packet_send << player_message_send;
	// Send it over the network (socket is a valid sf::TcpSocket)
	switch (socket->send(packet_send, *ip_address, *port)) 
	{
		case sf::Socket::Done:
			// Received a packet.
			if (debug_mode) std::cout << "CLIENT: Got one!\n";
			break;

		case sf::Socket::NotReady:
			// No more data to receive (yet).
			if (debug_mode) std::cout << "CLIENT: No more data to receive now\n";
			return;

		default:
			// Something went wrong.
			if (debug_mode) std::cout << "CLIENT: receive didn't return Done\n";
			return;
	}


	/// don't need to clear the packet since all the local variables 
	/// cease to exist once the function is over but it's good to 
	/// keep in mind that if the packet is static it should get cleared
	/// after each use
	/// packet_send.clear();
}

////////////////////////////////////////////////////////////
// ...wait for the answer
//
////////////////////////////////////////////////////////////
void GameClient::checkForIncomingPackets()
{
	while (true) {
		// Try to receive the packet from the other end
		// SEND (to the server) MUST MATCH packet_send in the GameServer
		sf::Packet packet_receive;
		sf::IpAddress sender;
		unsigned short senderPort;
		switch (socket->receive(packet_receive, sender, senderPort))
		{
		case sf::Socket::Done:
			// Received a packet.
			if (debug_mode) std::cout << "CLIENT: Got one!\n";
			break;

		case sf::Socket::NotReady:
			// No more data to receive (yet).
			if (debug_mode) std::cout << "CLIENT: No more data to receive now\n";
			return;

		default:
			// Something went wrong.
			if (debug_mode) std::cout << "CLIENT: receive didn't return Done\n";
			return;
		}

		// Extract the variables contained in the packet
		// Packets must match to what the server is sending (e.g.: server is sending string, client must expect string)
		PlayerMessage player_message_receive;
		if (packet_receive >> player_message_receive)
		{
			// Data extracted successfully...
			if (debug_message) displayMessage(player_message_receive);
			// Deal with the messages from the packet
			// Put position into history of network positions
			sf::Vector2f net_player_position(player_message_receive.x, player_message_receive.y);

			if (network_positions.size() > 3) network_positions.pop();
				network_positions.push(net_player_position);
			
			player.setPosition(net_player_position);
		}
	}
}

void GameClient::update()
{
	//std::thread st1(call_once_set_window, *window);
	//st1.join();
	call_once_set_window();

	//fps = 1.f / dt;
	//text.setString(std::to_string(fps));
	if (fps > 60) fps = 0;

	if (!hasStarted)
	{
		audioMgr.playMusicbyName("cantina");
		hasStarted = true;
	}

	/*if (input->isKeyDown(sf::Keyboard::Up))
	{
		input->setKeyUp(sf::Keyboard::Up);
		player.jump();
		audioMgr.playSoundbyName("jump");
	}*/

	//player.update(dt);

	if (input->isKeyDown(sf::Keyboard::Num1))
	{
		input->setKeyUp(sf::Keyboard::Num1);
		audioMgr.playSoundbyName("up");
	}

	if (input->isKeyDown(sf::Keyboard::Num2))
	{
		input->setKeyUp(sf::Keyboard::Num2);
		audioMgr.playSoundbyName("getover");
	}
	if (input->isKeyDown(sf::Keyboard::Num3))
	{
		input->setKeyUp(sf::Keyboard::Num3);
		audioMgr.playSoundbyName("glass");
	}
	if (input->isKeyDown(sf::Keyboard::BackSpace))
	{
		input->setKeyUp(sf::Keyboard::BackSpace);
		audioMgr.stopAllMusic();
	}
	if (input->isKeyDown(sf::Keyboard::Num4))
	{
		input->setKeyUp(sf::Keyboard::Num4);
		audioMgr.playMusicbyName("hyrule");
	}

	// check collision with world
	//std::vector<Tile>* world = level.getLevel();
	//for (int i = 0; i < (int)world->size(); i++)
	//{
	//	if ((*world)[i].isAlive())
	//	{
	//		// if "alive" check collision
	//		// world tile which are not alive don't want collision checks
	//		if (checkCollision(&player, &(*world)[i]))
	//		{
	//			player.collisionRespone(&(*world)[i]);
	//		}
	//	}
	//}

	/*if (player.getPosition().y > window->getSize().y)
	{
		*state = GameState::NETWORK;
		player.setPosition(0, 0);
		hasStarted = false;
		audioMgr.stopAllSounds();
		audioMgr.stopAllMusic();
	}
	else
	{
		*state = GameState::GAME_CLIENT;
	}*/


	// send packets at 10Hz rate (at 10PFS)
	if ((int)fps % 6 == 0)
		sendPacket();

	checkForIncomingPackets();

	// TODO keep track of local positions

	// TODO lerp

	// TODO add lerp to local positions

	/*const int msize = messages_.size();
	assert(msize >= 3);
	const TankMessage& msg0 = messages_[msize - 1];
	const TankMessage& msg1 = messages_[msize - 2];
	const TankMessage& msg2 = messages_[msize - 3];*/

	// FIXME: Implement prediction here!
	// You have:
	// - the history of position messages received, in "messages_"
	//   (msg0 is the most recent, msg1 the 2nd most recent, msg2 the 3rd most recent)
	// - the current time, in "time"
	// You need to update:
	// - the predicted position at the current time, in "x_" and "y_"

	//float x_average_velocity, y_average_velocity;

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	//x_average_velocity = (msg0.x - msg1.x) / (msg0.time - msg1.time);
	//y_average_velocity = (msg0.y - msg1.y) / (msg0.time - msg1.time);

	// linear model
	//x_ = x_average_velocity * (time - msg1.time) + msg1.x;
	//y_ = y_average_velocity * (time - msg1.time) + msg1.y;

	//// quadratic model
	//float 
	//	x_average_velocity_1,
	//	y_average_velocity_1, 
	//	x_average_velocity_2, 
	//	y_average_velocity_2, 
	//	a_x, 
	//	a_y;

	//// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	//x_average_velocity_1 = (msg0.x - msg1.x) / (msg0.time - msg1.time);
	//y_average_velocity_1 = (msg0.y - msg1.y) / (msg0.time - msg1.time);

	//x_average_velocity_2 = (msg1.x - msg2.x) / (msg1.time - msg2.time);
	//y_average_velocity_2 = (msg1.y - msg2.y) / (msg1.time - msg2.time);

	//a_x = (x_average_velocity_2 - x_average_velocity_1);
	//a_y = (y_average_velocity_2 - y_average_velocity_1);

	//// s = s0 + v0t + ½at2
	//x_ = msg2.x + (x_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_x) * powf((time - msg2.time), 2));
	//y_ = msg2.y + (y_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_y) * powf((time - msg2.time), 2));

	// increase fps
	fps++;
}
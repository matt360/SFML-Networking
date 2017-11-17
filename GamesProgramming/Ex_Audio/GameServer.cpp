#include "GameServer.h"
#include "States.h"

GameServer::GameServer(sf::RenderWindow* hwnd, Input* in, GameState* st, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short *port_number)
{
	window = hwnd;
	input = in;
	socket = udp_socket;
	ip_address = ip;
	port = port_number;
	state = st;
	// 

	fps = 0;
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
	/*std::vector<int> map = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 3, 0, 0, 0, 0, 0,
		1, 2, 3, 4, 6, 1, 2, 3, 0, 0,
		4, 5, 6, 0, 0, 4, 5, 6, 0, 0
	};*/

	std::vector<int> map = {
	1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 
	4, 6, 0, 0, 1, 3, 0, 0, 0, 0,
	4, 6, 0, 0, 4, 6, 1, 3, 0, 0,
	4, 6, 1, 3, 4, 6, 4, 6, 0, 0,
	4, 6, 4, 6, 4, 6, 4, 6, 1, 3,
	4, 6, 4, 6, 4, 6, 4, 6, 4, 6,
	};

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

	//int err = buff.loadFromFile("sfx/cantina.ogg");
	//soun.setBuffer(buff);
	//soun.play();
	//sf::Music music;
}

GameServer::~GameServer()
{

}

void GameServer::handleInput(float dt)
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.
}

void GameServer::render()
{
	beginDraw();

	level.render(window);
	window->draw(player);
	//window->draw(text);

	endDraw();
}

void GameServer::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void GameServer::endDraw()
{
	window->display();
}

// check AABB
bool GameServer::checkCollision(Sprite* s1, Sprite* s2)
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
bool GameServer::checkSphereBounding(Sprite* s1, Sprite* s2)
{
	// half width, height, give us the centre of the shape
	if (pow(s2->getPosition().x - s1->getPosition().x, 2) + pow(s2->getPosition().y - s1->getPosition().y, 2) < pow((s2->getSize().x / 2) + (s1->getSize().x / 2), 2))
	{
		return true;
	}
	return false;
}

void GameServer::addMessage(PlayerMessage& player_message_send)
{
	//PlayerMessage player_message_send;
	player_message_send.id = 0;
	player_message_send.x = player.getPosition().x;
	player_message_send.y = player.getPosition().y;
	player_message_send.time = 1.0;
}

//////////////////////////////////////////////////////////
/// Launch a server, wait for a message, send an answer.
///
//////////////////////////////////////////////////////////
void GameServer::runUdpServer()
{
	// Wait for a message
	// Receive the packet at the other end
	sf::Packet packet_receive;
	sf::IpAddress sender;
	unsigned short senderPort;
	if (socket->receive(packet_receive, sender, senderPort) != sf::Socket::Done)
		return;
	// Extract the variables contained in the packet
	// RECEIVE (from the client) MUST MATCH packet_send in the GameClient
	/*sf::Uint32 x_r;
	std::string s_r;
	double d_r;*/
	PlayerMessage player_message_receive;
	//if (packet_receive >> x_r >> s_r >> d_r)
	if (packet_receive >> player_message_receive)
	{
		// The message from the client
		std::cout << "\n\nSERVER: Message received from the client:";
		// Data extracted successfully...
		std::cout << "\nSERVER: ID: " << player_message_receive.id
			<< "\nSERVER: Player x: " << player_message_receive.x
			<< "\nSERVER: Player y: " << player_message_receive.y
			<< "\nSERVER: Time: " << player_message_receive.time;
		std::cout << "\nSERVER: client's IP: " << sender;
		std::cout << "\nSERVER: client's port: " << senderPort;
	}

	// SEND (to the client) MUST MATCH packet_receive in the GameClient
	sf::Packet packet_send;
	// Message to send
	PlayerMessage player_message_send;
	//player_message_send.id = 0;
	//player_message_send.x = player.getPosition().x;
	//player_message_send.y = player.getPosition().y;
	//player_message_send.time = 1.0;
	//std::string s = "Hi! I'm the server!";
	addMessage(player_message_send);
	// Group the variables to send into a packet
	packet_send << player_message_send;
	// Send it over the network (socket is a valid sf::TcpSocket)

	if (socket->send(packet_send, sender, senderPort) != sf::Socket::Done)
		return;

	// DEBUG purposes
	// Extract the variables contained in the packet
	//std::string s_s;
	PlayerMessage player_message_send_d;

	if (packet_send >> player_message_send_d)
	{
		// Data extracted successfully...
		// The message sent to the client
		std::cout << "\n\nSERVER: Message sent to the client:";
		std::cout << "\nSERVER: ID: " << player_message_send_d.id
			<< "\nSERVER: Player x: " << player_message_send_d.x
			<< "\nSERVER: Player y: " << player_message_send_d.y
			<< "\nSERVER: Time: " << player_message_send_d.time;
	}
}
void GameServer::update(float dt)
{
	fps = 1.f / dt;
	//text.setString(std::to_string(fps));
	//std::cout << fps << std::endl;

	if (!hasStarted)
	{
		audioMgr.playMusicbyName("cantina");
		hasStarted = true;
	}

	if (input->isKeyDown(sf::Keyboard::Up))
	{
		input->setKeyUp(sf::Keyboard::Up);
		player.jump();
		audioMgr.playSoundbyName("jump");
	}

	player.update(dt);

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
	std::vector<Tile>* world = level.getLevel();
	for (int i = 0; i < (int)world->size(); i++)
	{
		if ((*world)[i].isAlive())
		{
			// if "alive" check collision
			// world tile which are not alive don't want collision checks
			if (checkCollision(&player, &(*world)[i]))
			{
				player.collisionRespone(&(*world)[i]);
			}
		}
	}

	if (player.getPosition().y > window->getSize().y)
	{
		*state = GameState::NETWORK;
		player.setPosition(0, 0);
		hasStarted = false;
		audioMgr.stopAllSounds();
		audioMgr.stopAllMusic();
	}
	else
	{
		*state = GameState::GAME_SERVER;
	}

	//if ((int)fps % 6 == 0)
	// server should probably keep listening and sending all the time
	runUdpServer();
}


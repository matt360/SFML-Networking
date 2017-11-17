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

void GameClient::handleInput(float dt)
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.
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

void GameClient::addMessage(PlayerMessage& player_message_send)
{
	//PlayerMessage player_message_send;
	player_message_send.id = 0;
	player_message_send.x = player.getPosition().x;
	player_message_send.y = player.getPosition().y;
	player_message_send.time = 1.0;
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
	if (socket->send(packet_send, *ip_address, *port) != sf::Socket::Done) {
		std::cout << "send failed\n"; // TODO do something better than this in real code ;-)
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
			std::cout << "CLIENT: Got one!\n";
			break;

		case sf::Socket::NotReady:
			// No more data to receive (yet).
			std::cout << "CLIENT: No more data to receive now\n";
			return;

		default:
			// Something went wrong.
			std::cout << "CLIENT: receive didn't return Done\n";
			return;
		}

		// Extract the variables contained in the packet
		// Packets must match to what the server is sending (e.g.: server is sending string, client must expect string)
		PlayerMessage player_message_receive;
		if (packet_receive >> player_message_receive)
		{
			// Data extracted successfully...
			// The message from the server
			std::cout << "\nCLIENT: Message received from the server:";
			// Data extracted successfully...
			std::cout << "\nCLIENT: ID: " << player_message_receive.id
				<< "\nCLIENT: Player x: " << player_message_receive.x
				<< "\nCLIENT: Player y: " << player_message_receive.y
				<< "\nCLIENT: Time: " << player_message_receive.time;
			
			//packet_receive.clear();
		}
	}
}

void GameClient::update(float dt)
{

	//fps = 1.f / dt;
	//text.setString(std::to_string(fps));
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
		*state = GameState::GAME_CLIENT;
	}

	// send packets at 10Hz rate (at 10PFS)
	if ((int)fps % 6 == 0)
		sendPacket();

	checkForIncomingPackets();
}
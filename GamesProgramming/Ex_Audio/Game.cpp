#include "Game.h"
#include "States.h"

Game::Game(sf::RenderWindow* hwnd, Input* in, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short *port_number)
{
	window = hwnd;
	input = in;
	socket = udp_socket;
	ip_address = ip;
	port = port_number;
	//state = GameState::LEVEL;
	// 
	//if (getNetworkState() == NetworkState::NONE) { networkState = NetworkState::CLIENT; }

	fps = 0;
	font.loadFromFile("font/arial.ttf");
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

Game::~Game()
{

}

GameState Game::getState()
{
	return state;
}

void Game::handleInput(float dt)
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.
}

void Game::render()
{
	beginDraw();
	
	level.render(window);
	window->draw(player);
	//window->draw(text);

	endDraw();
}

void Game::beginDraw()
{
	sf::Color col;
	col.r = 100;
	col.g = 149;
	col.b = 237;
	window->clear(col);
}

void Game::endDraw()
{
	window->display();
}

// check AABB
bool Game::checkCollision(Sprite* s1, Sprite* s2)
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
bool Game::checkSphereBounding(Sprite* s1, Sprite* s2)
{
	// half width, height, give us the centre of the shape
	if (pow(s2->getPosition().x - s1->getPosition().x, 2) + pow(s2->getPosition().y - s1->getPosition().y, 2) < pow((s2->getSize().x / 2) + (s1->getSize().x / 2), 2))
	{
		return true;
	}
	return false;
}





//void Game::do_once()
//{
//	std::call_once(ask_flag, [&]() {
//		if (getNetworkState() == NetworkState::SERVER) {
//			who = 's';
//		}
//		else {
//			who = 'c';
//		}
//	}
//	);
//}


//////////////////////////////////////////////////////////////
///// Launch a server, wait for a message, send an answer.
/////
//////////////////////////////////////////////////////////////
//void Game::runUdpServer()
//{
//	// Wait for a message
//	char in[128];
//	std::size_t received;
//	sf::IpAddress sender;
//	unsigned short senderPort;
//	if (socket->receive(in, sizeof(in), received, sender, senderPort) != sf::Socket::Done)
//		return;
//	std::cout << "Message received from client " << sender << ": \"" << in << "\"" << std::endl;
//
//	// Send an answer to the client
//	const char out[] = "Hi, I'm the server";
//	if (socket->send(out, sizeof(out), sender, senderPort) != sf::Socket::Done)
//		return;
//	std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;
//}

//////////////////////////////////////////////////////////////
///// Send a message to the server, wait for the answer
/////
//////////////////////////////////////////////////////////////
//void Game::runUdpClient()
//{
//	// Send a message to the server
//	const char out[] = "Hi, I'm a client";
//
//	switch (socket->send(out, sizeof(out), *ip_address, *port))
//	{
//	case sf::Socket::NotReady:
//		std::cout << "Socket not ready " << *ip_address << std::endl;
//		break;
//
//	case sf::Socket::Done:
//		std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
//		break;
//
//	case sf::Socket::Disconnected:
//		std::cout << "Disconnected" << std::endl;
//		break;
//
//	case sf::Socket::Error:
//		std::cout << "Socket Error" << std::endl;
//		break;
//
//	default:
//		std::cout << "Default Error" << std::endl;
//		return;
//	}
//
//	// Receive an answer from anyone (but most likely from the server)
//	char in[128];
//	std::size_t received;
//	sf::IpAddress sender;
//	unsigned short senderPort;
//	switch (socket->receive(in, sizeof(in), received, sender, senderPort))
//	{
//	case sf::Socket::NotReady:
//		std::cout << "Socket not ready " << *ip_address << std::endl;
//		break;
//
//	case sf::Socket::Done:
//		std::cout << "Message received from " << sender << ": \"" << in << "\"" << std::endl;
//		break;
//
//	case sf::Socket::Disconnected:
//		std::cout << "Disconnected" << std::endl;
//		break;
//
//	case sf::Socket::Error:
//		std::cout << "Socket Error" << std::endl;
//		break;
//
//	default:
//		std::cout << "Default Error" << std::endl;
//		return;
//	}
//}
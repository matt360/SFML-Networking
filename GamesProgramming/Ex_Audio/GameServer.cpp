#include "GameServer.h"



GameServer::GameServer()
{
}


GameServer::~GameServer()
{
}

////////////////////////////////////////////////////////////
/// Launch a server, wait for a message, send an answer.
///
////////////////////////////////////////////////////////////
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
	sf::Uint32 x_r;
	std::string s_r;
	double d_r;
	if (packet_receive >> x_r >> s_r >> d_r)
	{
		// Data extracted successfully...
		std::cout << "SERVER: Message received from client " << sender << std::endl;
		// Data extracted successfully...
		std::cout << "\nserver x: " << x_r << "\nserver s: " << s_r << "\nserver d: " << d_r << std::endl;
	}

	/*char in[128];
	std::size_t received;
	sf::IpAddress sender;
	unsigned short senderPort;
	if (socket->receive(in, sizeof(in), received, sender, senderPort) != sf::Socket::Done)
	return;
	std::cout << "Message received from client " << sender << ": \"" << in << "\"" << std::endl;*/

	// Send an answer to the client
	/*sf::Uint32 x = 24;
	std::string s = "hello";
	double d = 5.89;*/
	// Group the variables to send into a packet
	sf::Packet packet_send;
	std::string s = "Hi! I'm the server!";
	packet_send << s;
	// Send it over the network (socket is a valid sf::TcpSocket)

	if (socket->send(packet_send, sender, senderPort) != sf::Socket::Done)
		return;
	// Extract the variables contained in the packet
	std::string s_s;
	if (packet_send >> s_s)
	{
		// Data extracted successfully...
		std::cout << "SERVER: Message sent to the client: ";
		std::cout << "server: s_s: " << s_s << std::endl;
	}


	//// Send an answer to the client
	//const char out[] = "Hi, I'm the server";
	//if (socket->send(out, sizeof(out), sender, senderPort) != sf::Socket::Done)
	//	return;
	//std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;
}

void GameServer::update(float dt)
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
		state = GameState::MENU;
		player.setPosition(0, 0);
		hasStarted = false;
		audioMgr.stopAllSounds();
		audioMgr.stopAllMusic();
	}
	else
	{
		state = GameState::LEVEL;
	}

	runUdpServer();
}
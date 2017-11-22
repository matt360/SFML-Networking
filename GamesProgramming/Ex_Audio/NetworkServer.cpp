#include "NetworkServer.h"

NetworkServer::NetworkServer(sf::RenderWindow* hwnd,
	Input* in,
	sf::Clock* cl) : Network()
{
	window = hwnd;
	input = in;
	clock = cl;

	established_connection = false;
	
	ready = false;
	server = false;
	client = false;
	debug_mode = false;
	debug_message = false;

	// Network text
	font.loadFromFile("font/advanced_pixel-7.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Press 'S' to be a server\n\nPress 'C' to be a client\n\nPress Enter to Play");
}

NetworkServer::~NetworkServer()
{
}

void NetworkServer::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

void NetworkServer::handleInput()
{
	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		// player ready to play
		ready = true;
	}

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

// error handling before connecting
// error while the game is running
// states
// what I want to send
// anything that happens to the client shouldn't affect the server player
// anything that happens to the server the client must handle accordingly - if the server is dead - try to reconnect for a ceratin time - take to the network state

void NetworkServer::establishConnectionWithClient()
{
	// Wait for a message
	// Receive the packet at the other end
	sf::Packet packet_receive;
	sf::IpAddress sender;
	unsigned short senderPort;
	switch (socket.receive(packet_receive, sender, senderPort))
	{
	case sf::Socket::Done:
		// Received a packet.
		if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
		return;
	}

	// Extract the variables contained in the packet
	// RECEIVE (from the client) MUST MATCH packet_send in the GameClient
	float hello;
	if (packet_receive >> hello)
	{
		// Deal with the messages from the packet
		// The message from the client
		established_connection = (int)hello;
		if (debug_message) displayReceiveMessage(hello);
	}

	//////////////////////////////////////////////////////////////////////
	// SEND (to the client) MUST MATCH packet_receive in the GameClient //
	//////////////////////////////////////////////////////////////////////
	sf::Packet packet_send;
	// Message to send
	// Group the variables to send into a packet
	//established_connection = true;
	sf::Int32 server_time = clock->getElapsedTime().asMilliseconds();
	packet_send << server_time << established_connection;
	std::cout << "server_time: " << server_time << "\n";
	//sf::sleep(sf::milliseconds(1000));
	// Send it over the network
	switch (socket.send(packet_send, sender, senderPort))
	{
	case sf::Socket::Done:
		// Received a packet.
		if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";

		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
		return;
	}

	// DEBUG purposes
	// Extract the variables contained in the packet
	if (debug_message)
	{
		if (packet_send >> server_time >> established_connection)
		{
			// Data extracted successfully...
			if (debug_message) displaySendMessage(server_time);
		}
	}
}

void NetworkServer::update()
{	
	text.setString("\n\nYou're the server\n\nWaiting for the client...\n\nPress Enter to Play");

	establishConnectionWithClient();

	sf::Int32 server_time = clock->getElapsedTime().asMilliseconds();
	std::cout << "server_time: " << server_time << "\n";

	if (ready && established_connection)
	{
		game_state = GameStateEnum::GAME_SERVER;
	}
}
#include "NetworkClient.h"
#include <iostream>

NetworkClient::NetworkClient(sf::RenderWindow* hwnd, Input* in, GameState* st, sf::UdpSocket* udp_socket, sf::IpAddress* ip, unsigned short* port_number, sf::Clock* cl, float* cur_t)
{
	window = hwnd;
	input = in;
	socket = udp_socket;
	ip_address = ip;
	port = port_number;
	state = st;
	clock = cl;
	current_time = cur_t;

	readyToPlay = false;
	server = false;
	client = false;
	debug_mode = true;
	debug_message = true;
	network_state = NetworkState::NONE;

	// Network text
	font.loadFromFile("font/advanced_pixel-7.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(200, 100);
	text.setString("Press 'S' to be a server\n\nPress 'C' to be a client\n\nPress Enter to Play");
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::handleInput()
{
	if (input->isKeyDown(sf::Keyboard::Return))
	{
		input->setKeyUp(sf::Keyboard::Return);
		// player ready to play
		readyToPlay = true;
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

void NetworkClient::displayMessage(sf::Int32 time)
{
	// The message from the server
	std::cout << "\n\nCLIENT: Message received from the server:";
	// Data extracted successfully...
	std::cout << "\nCLIENT: server's time: " << time;
}

void NetworkClient::displayMessage(sf::Int32 time, const sf::IpAddress sender, const unsigned short sender_port)
{
	// The message from the server
	std::cout << "\nCLIENT: server's IP: " << sender;
	std::cout << "\nCLIENT: server's port: " << sender_port;
	std::cout << "\nCLIENT: server's time: " << time;
}

////////////////////////////////////////////////////////////
// Send a message to the server...
//
////////////////////////////////////////////////////////////
void NetworkClient::sendPacketToServer()
{
	// message
	// RECEIVE (what server receives) - MUST MATCH packet_receive in the GameServer
	client_send_time = clock->getElapsedTime().asMilliseconds();
	// Group the variables to send into a packet
	sf::Packet packet_send;
	//addMessage(player_message_send);
	packet_send << client_send_time;
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
void NetworkClient::checkForIncomingPacketsFromServer()
{
	////////////////////////////////////////////////////////////////////
	// CHECK FOR INCOMING PACKETS                                     //
	////////////////////////////////////////////////////////////////////
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

		sf::Int32 receive_time;
		// Extract the variables contained in the packet
		// Packets must match to what the server is sending (e.g.: server is sending string, client must expect string)
		if (packet_receive >> receive_time)
		{
			// Data extracted successfully...
			if (debug_message) displayMessage(receive_time);
			// Deal with the messages from the packet
			client_receive_time = receive_time;
		}
	}
}


void NetworkClient::establishConnectionWithServer()
{
	// send message to the server...
	sendPacketToServer();

	// ...wait for the answer
	checkForIncomingPacketsFromServer();
}

void NetworkClient::update()
{
	// Client or server ?
	if (server)
	{
		// create server socket
		//createServerSocket();
		text.setString("Connecting...\n\nYou're the server\n\nPress Enter to Play");

		server = false;
	}
	if (client)
	{
		// create client socket
		//createClientSocket();
		// message - joined the server
		text.setString("Connecting...\n\nYou're the client\n\nPress Enter to Play");

		client = false;
	}

	switch (network_state)
	{
	case (NetworkState::SERVER):
		//establishConnectionWithClient();
		break;
	case (NetworkState::CLIENT):
		establishConnectionWithServer();
		break;
		/*default:

		break;*/
	}

	if (readyToPlay)
	{
		switch (network_state)
		{
		case (NetworkState::SERVER):
			*state = GameState::GAME_SERVER;
			break;
		case (NetworkState::CLIENT):
			*state = GameState::GAME_CLIENT;
			break;
		default:
			readyToPlay = false;
			break;
		}
	}
}

void NetworkClient::render()
{
	beginDraw();

	window->draw(text);

	endDraw();
}

void NetworkClient::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void NetworkClient::endDraw()
{
	window->display();
}

//void Network::do_once()
//{
//	std::call_once(ask_flag, [&]() {
//		std::cout << "Do you want to be a server (s) or a client (c)? ";
//		//std::cin >> who; 
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
//			networkState = NetworkState::SERVER;
//		}
//		else {
//			networkState = NetworkState::CLIENT;
//		}
//	}
//	);
//}
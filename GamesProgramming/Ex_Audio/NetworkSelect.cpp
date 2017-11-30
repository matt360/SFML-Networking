#include "NetworkSelect.h"

NetworkSelect::NetworkSelect() {}

NetworkSelect::~NetworkSelect()
{
}

// error handling before connecting
// error while the game is running
// states
// what I want to send
// anything that happens to the client shouldn't affect the server player
// anything that happens to the server the client must handle accordingly - if the server is dead - try to reconnect for a ceratin time - take to the network state

void NetworkSelect::createServerSocket()
{
	//////////////////////////////////////////
	// Create a socket to receive a message from anyone
	// unbinding the socket prevents the socket binding failure if the person tries to bind the port more than once.
	// it also assignes a random port number
	socket.unbind();
	socket.setBlocking(false);
	// Listen to messages on the aumatically assigned port
	if (socket.bind(port) != sf::Socket::Done) return;

	//if (debug_mode) std::cout << "Server is listening to port " << port << ", waiting for a message...\n";
	//////////////////////////////////////////
}

void NetworkSelect::createClientSocket()
{
	//////////////////////////////////////////
	socket.unbind();
	socket.setBlocking(false);

	// Ask for the server address
	/*do
	{
	std::cout << "Type the address or name of the server to connect to: ";
	std::cin >> server;
	} while (server == sf::IpAddress::None);*/
	///////////////////////////////////////////
}
#include "NetworkSelect.h"

NetworkSelect::NetworkSelect() {}

NetworkSelect::~NetworkSelect()
{
}

void NetworkSelect::createServerSocket()
{
	// Create a socket to receive a message from anyone
	// unbinding the socket prevents the socket binding failure if bind called more than once.
	socket.setBlocking(false);
	// bind the socket
	if (socket.bind(port) != sf::Socket::Done) return;
}

void NetworkSelect::createClientSocket()
{
	socket.setBlocking(false);
}
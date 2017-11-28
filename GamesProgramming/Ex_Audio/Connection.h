///* The Connection class */
//#pragma once
//#include <SFML/Network.hpp>
//#include <SFML/Main.hpp>
//#include "Network.h"
//#include <WinSock2.h>
//
//// Each instance of this class represents a connected client.
//class Connection : public Network
//{
//public:
//	// Constructor.
//	// sock: the socket that we've accepted the client connection on.
//	Connection(SOCKET sock);
//	Connection();
//
//	// Destructor.
//	~Connection();
//
//	/// socket
//	//static sf::UdpSocket socket;
//	/// Choose an arbitrary port for opening sockets
//	//unsigned short port;
//	/// IP adrress to connect to
//	//sf::IpAddress ip_address;
//	/// connection handler - does not let the server start without establishing the connection with the client
//	//bool established_connection;
//	
//	/// get cthe current time and take the offset into account
//	//sf::Int32 getCurrentTime(const sf::Clock& clock, const sf::Int32& offset);
//
//	// Process an incoming message.
//	void processMessage(const NetMessage *message);
//
//	// Add an outgoing message to the send buffer.
//	void sendMessage(const NetMessage *message);
//
//	// Return the client's socket.
//	SOCKET sock();
//
//	// Return whether this connection is in a state where we want to try
//	// reading from the socket.
//	bool wantRead();
//
//	// Return whether this connection is in a state where we want to try
//	// writing to the socket.
//	bool wantWrite();
//
//	// Call this when the socket is ready to read.
//	// Returns true if the socket should be closed.
//	bool doRead();
//
//	// Call this when the socket is ready to write.
//	// Returns true if the socket should be closed.
//	bool doWrite();
//
//private:
//	// This client's socket.
//	SOCKET sock_;
//
//	// The data we've read from the client.
//	char readBuffer_[sizeof NetMessage];
//	int readCount_;
//
//	// Data we need to send to the client.
//	char writeBuffer_[100 * sizeof NetMessage];
//	int writeCount_;
//};
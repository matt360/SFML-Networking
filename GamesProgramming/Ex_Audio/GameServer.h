#pragma once
#include "Game.h"
class GameServer :
	public Game
{
public:
	GameServer();
	~GameServer();

	// Network
	void update(float dt);

private:
	void runUdpServer();
};


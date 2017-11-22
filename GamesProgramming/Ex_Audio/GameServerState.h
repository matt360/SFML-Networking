#pragma once
#include "Network.h"
#include "GameState.h"

class GameServerState : public GameState, public Network
{
public:
	GameServerState(sf::RenderWindow* hwnd, Input* in);
	~GameServerState();

	virtual void handleInput();
	virtual void update();
	virtual void render();
	//Window* getWindow();

private:
	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);
	//void moveMushroom();
	//Window m_window;

	virtual void addMessage(PlayerMessage& player_message);
	void runUdpServer();

	// window
	void beginDraw();
	void endDraw();
};
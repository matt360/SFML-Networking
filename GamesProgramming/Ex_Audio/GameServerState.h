#pragma once
#include "GameServerNetwork.h"
#include "GameState.h"

class GameServerState : public GameState, public GameServerNetwork
{
public:
	GameServerState(sf::RenderWindow* hwnd, Input* in);
	~GameServerState();

	virtual void handleInput();
	virtual void update();
	virtual void render();
	//Window* getWindow();

private:
	//void moveMushroom();
	//Window m_window;
	/////////////////////////// ESTABLISH CONNECTION WITH THE CLIENT ///////////////////////////////
	void establishConnectionWithClient(const bool& debug_mode);


	// window
	void beginDraw();
	void endDraw();

	bool debug_mode;
	bool debug_message;
};
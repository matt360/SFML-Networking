#pragma once
#include "GameClient.h"
#include "GameState.h"

// TODO It should be possible to go back from the client game state to client network state (GameClientState -> NetworkClientState)

class GameClientState : public GameState, public GameClient 
{
public:
	GameClientState(sf::RenderWindow* hwnd, Input* in);
	~GameClientState();

	virtual void handleInput();
	virtual void update();
	virtual void render();
	//Window* getWindow();

private:
	//void moveMushroom();
	//Window m_window;

	// window
	void beginDraw();
	void endDraw();

	// toggle lerp mode - input handler
	bool lerp_mode;
	bool linear_prediction;
	bool quadratic_prediction;

	// toggle debug - input handler
	bool debug_mode;
	bool debug_message;
};



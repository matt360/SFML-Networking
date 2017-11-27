#pragma once
#include "GameClientNetwork.h"
#include "GameState.h"

class GameClientState : public GameState, public GameClientNetwork
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

	// for displaying text
	sf::Text text;

	// window
	void beginDraw();
	void endDraw();

	// toggle debug - input handler
	bool debug_mode;
	bool debug_message;
};



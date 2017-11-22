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

	float lerp(float start, float end, float time);
	sf::Vector2f lerp(const sf::Vector2f & start, const sf::Vector2f & end, const float time);

	// debug
	bool lerp_mode;
};



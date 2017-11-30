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
	sf::Int32 server_time;
	sf::Int32 lag = 1000; // in milliseconds
	sf::Int32 start_timing_latency;
	sf::Int32 end_timing_latency;
	sf::Int32 latency;

	bool send_packet = true;
	void sendPacketToServer(const bool& debug_mode);
	void checkForIncomingPacketsFromServer(const bool& debug_mode);
	void establishConnectionWithServer(const bool& debug_mode);
	
	// window
	void beginDraw();
	void endDraw();

	// toggle debug - input handler
	bool debug_mode;
	bool debug_message;
};



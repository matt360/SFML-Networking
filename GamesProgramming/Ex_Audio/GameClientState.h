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
	sf::Int32 lag = 100; // in milliseconds
	sf::Int32 start_timing_latency;
	sf::Int32 end_timing_latency;
	sf::Int32 latency;

	// keep track of the current ip address
	//sf::IpAddress ip_address;
	// keep track of the current port
	//unsigned short port;

	bool clocks_synced = false;
	void sendPacketToServer(const bool& debug_mode);
	void checkForIncomingPacketsFromServer(const bool& debug_mode);
	void establishConnectionWithServer(const bool& debug_mode);
	
	// window
	void beginDraw();
	void endDraw();

	// toggle debug - input handler
	bool debug_mode;
	bool debug_message;

	//inline float clamp(float n, float lower, float upper) { return std::fmax(lower, (std::fmin(n, upper))); }
};



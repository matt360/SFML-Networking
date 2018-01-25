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

	void keepTrackOfPlayerLocalPositions();
	void keepTrackOfEnemyLocalPositions();

	void playerLinearPrediction();
	void enemyLinearPrediciton();
	void playerQuadraticPrediction();
	void enemyQuadraticPrediction();

	bool clocks_synced = false;
	// send a hello meesage to the server and then run syncClockWithServer only once
	void sayHelloToServer(const bool& debug_mode);
	void syncClockWithServer(const bool& debug_mode);
	void syncClocks(const bool& debug_mode);
	
	// display text
	void displayText();
	// window
	void beginDraw();
	void endDraw();

	// toggle debug - input handler
	bool debug_mode;

	//inline float clamp(float n, float lower, float upper) { return std::fmax(lower, (std::fmin(n, upper))); }
};



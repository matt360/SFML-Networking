#pragma once
#include "Network.h"
#include "GameState.h"

class GameServer : public GameState, public Network
{
public:
	GameServer(sf::RenderWindow* hwnd,
		sf::Clock* clock,
		sf::Int32* offset);
	~GameServer();

	virtual void handleInput();
	virtual void update();
	virtual void render();
	//Window* getWindow();

private:
	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);
	//void moveMushroom();
	//Window m_window;
	/*sf::RenderWindow* window;*/
	// Network //////////////////////
	/*sf::Clock* clock;
	sf::Int32* offset;
*/
	virtual void addMessage(PlayerMessage& player_message);
	void runUdpServer();

	// window

	void beginDraw();
	void endDraw();
	/////////////////////////////////

	
};
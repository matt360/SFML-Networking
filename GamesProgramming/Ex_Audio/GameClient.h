#pragma once
#include <deque>
#include "Network.h"
#include "GameState.h"

// TODO It should be possible to go back from the client game state to client network state (GameClient -> NetworkClient)

class GameClient : public GameState, public Network 
{
public:
	GameClient(sf::RenderWindow* hwnd, Input* in);
	~GameClient();

	virtual void handleInput();
	virtual void update();
	virtual void render();
	//Window* getWindow();

private:
	bool checkCollision(Sprite* s1, Sprite* s2);
	bool checkSphereBounding(Sprite* s1, Sprite* s2);
	//void moveMushroom();
	//Window m_window;
	
	const int num_messages = 1; // 1 - for linear interpolation, 2 - for quadratic interpolation
	std::deque<PlayerMessage> local_positions;
	void keepTrackOfLocalPositoins();
	void keepTrackOfLocalPositoins(sf::Vector2f& vec);
	void keepTrackOfNetworkPositions(const PlayerMessage& player_message_receive);
	std::deque<PlayerMessage> network_positions;

	sf::Vector2f predict_local_path();
	sf::Vector2f predict_network_path();

	virtual void addMessage(PlayerMessage& player_message);
	void sendPacket();
	void checkForIncomingPackets();
	/*void addMessage(PlayerMessage& player_message);
	void displayMessage(const PlayerMessage player_message);
	void displayMessage(const PlayerMessage player_message, const sf::IpAddress ip_address, const unsigned short port);*/

	// window
	void beginDraw();
	void endDraw();

	sf::Int32 getCurrentTime();

	float lerp(float start, float end, float time);

	sf::Vector2f lerp(const sf::Vector2f & start, const sf::Vector2f & end, const float time);

	// debug
	bool lerp_mode;
	/////////////////////////////////

	/*float fps;
	sf::Text text;
	sf::Font font;*/


	//AudioManager audioMgr;

	//bool hasStarted;

	//// Game Variables
	//Player player;
	////Enemy enemy;
	////Cursor cursor;
	//sf::Texture texture;

	//Map level;

	//// sound test
	//sf::SoundBuffer buff;
	//sf::Sound soun;
};



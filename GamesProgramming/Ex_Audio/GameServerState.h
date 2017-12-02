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
	void syncClocksWithClient(const bool& debug_mode);

	//std::set<unsigned short> addresses;
	std::map<unsigned short, unsigned short> addresses;
	//std::vector<unsigned short> addresses_v;
	int latestID = 0;
	// is there a new connection?
	bool new_connection = false;
	unsigned short hertz = 0;
	unsigned short hertz_rate = 0;
	std::string hertz_string = "";
	void setHertz();
	
	// display text
	void displayText();
	//
	void keepSpriteWithinWindow(Sprite& sprite);

	// window
	void beginDraw();
	void endDraw();

	bool debug_mode;
	bool debug_message;

	inline float lerp(float start, float end, float time)
	{
		return start * (1.0f - time) + time * end;
	}

	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float& time)
	{
		sf::Vector2f temp;
		temp.x = lerp(start.x, end.x, time);
		temp.y = lerp(start.y, end.y, time);
		return temp;
	}
};
#pragma once

#include <SFML/Graphics.hpp>

class Game {
public:
	Game(sf::RenderWindow* hwnd);
	~Game();

	void handleInput();
	void update();
	void render();
	//Window* getWindow();


private:
	//void moveMushroom();
	//Window m_window;
	sf::RenderWindow* window;
	void beginDraw();
	void endDraw();

	// Game Variables
	sf::RectangleShape rect;
	sf::CircleShape circle;
	
};
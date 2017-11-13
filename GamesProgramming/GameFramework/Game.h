#pragma once
#include "Window.h"

class Game {
public:
	Game();
	~Game();

	void handleInput();
	void update();
	void render();
	Window* getWindow();

private:
	void moveMushroom();
	Window m_window;

	// Game Variables
	sf::Sprite m_mushroom;
	sf::Sprite m_mushroom2;
	sf::Texture m_mushroomTexture;
	sf::Texture m_mushroomTexture2;
	sf::Vector2f increment;
};
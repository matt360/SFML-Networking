// Map Class
// Builds and store level sections based on TileMap and TileSet

#pragma once
#include <math.h>
#include "Tile.h"

class Map
{
public:
	Map();
	~Map();

	void loadTexture(char* filename);
	void setTileSet(std::vector<Tile> ts);
	void setTileMap(std::vector<int> tm, sf::Vector2u mapDimensions);
	void buildLevel();

	void render(sf::RenderWindow* window);
	std::vector<Tile>* getLevel(){ return &level; };

	void setPosition(sf::Vector2f pos) { position = pos; };

protected:
	std::vector<Tile> tileSet;
	std::vector<int> tileMap;
	std::vector<Tile> level;
	sf::Texture texture;
	sf::Vector2u mapSize;
	sf::Vector2f position;
};


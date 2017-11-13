#include "Game.h"

Game::Game(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	fps = 0;
	font.loadFromFile("font/arial.ttf");
	text.setFont(font);
	text.setCharacterSize(18);
	text.setString("hellow world");
	
	texture.loadFromFile("gfx/MushroomTrans.png");

	player.setSize(sf::Vector2f(32, 32));
	player.setTexture(&texture);
	player.setPosition(5, 5);
	player.setInput(input);
	player.setVelocity(0, 10);

	//window->setMouseCursorVisible(false);
	
	level.loadTexture("gfx/marioTiles.png");
	Tile tile;
	std::vector<Tile> tiles;
	//187, 51
	for (int i = 0; i < 7; i++)
	{
		
		tile.setSize(sf::Vector2f(32, 32));
		tile.setAlive(true);
		tiles.push_back(tile);
	}
	
	tiles[0].setAlive(false);
	tiles[0].setTextureRect(sf::IntRect(187, 51, 16, 16));
	tiles[1].setTextureRect(sf::IntRect(0, 0, 16, 16));
	tiles[2].setTextureRect(sf::IntRect(17, 0, 16, 16));
	tiles[3].setTextureRect(sf::IntRect(34, 0, 16, 16));
	tiles[4].setTextureRect(sf::IntRect(0, 34, 16, 16));
	tiles[5].setTextureRect(sf::IntRect(17, 34, 16, 16));
	tiles[6].setTextureRect(sf::IntRect(34, 34, 16, 16));

	level.setTileSet(tiles);

	// Map dimensions
	sf::Vector2u mapSize(10, 6);

	// build map
	std::vector<int> map = {
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 1, 3, 0, 0, 0, 0, 0,
		 1, 2, 3, 4, 6, 1, 2, 3, 0, 0,
		 4, 5, 6, 0, 0, 4, 5, 6, 0, 0
	};

	/*std::vector<int> map = {
		1, 3, 0, 0, 0, 0, 0, 0, 0, 0,
		4, 6, 1, 3, 0, 0, 0, 0, 0, 0,
		0, 0, 4, 6, 1, 3, 0, 0, 0, 0,
		0, 0, 0, 0, 4, 6, 1, 3, 0, 0,
		0, 0, 0, 0, 0, 0, 4, 6, 1, 3,
		0, 0, 0, 0, 0, 0, 0, 0, 4, 6
	};*/

	level.setTileMap(map, mapSize);
	level.setPosition(sf::Vector2f(0, 408));
	level.buildLevel();

}

Game::~Game()
{

}

void Game::update(float dt)
{
	fps = 1.f / dt;
	text.setString(std::to_string(fps));
	//if (checkCollision(&ball4, &paddle))
	//{
	//	// if true collision occured
	//	ball4.collisionResponse(&paddle);
	//}

	//square1.update(dt);
	//square2.update(dt);
	//if (checkCollision(&square1, &square2))
	//{
	//	//square1.collisionResponse(&square2);
	//	//square2.collisionResponse(&square1);
	//}
	//if (checkSphereBounding(&square1, &square2))
	//{
	//	square1.collisionResponse(&square2);
	//	square2.collisionResponse(&square1);
	//}


	// check collision with world
	std::vector<Tile>* world = level.getLevel();
	for (int i = 0; i < (int)world->size(); i++)
	{
		if ((*world)[i].isAlive())
		{
			// if "alive" check collision
			// world tile which are not alive don't want collision checks
			if (checkCollision(&player, &(*world)[i]))
			{
				player.collisionRespone(&(*world)[i]);
			}
		}
	}

}

void Game::handleInput(float dt)
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.

	player.update(dt);

}


void Game::render()
{
	beginDraw();
	
	level.render(window);
	window->draw(player);
	//window->draw(text);

	endDraw();
}

void Game::beginDraw()
{
	sf::Color col;
	col.r = 100;
	col.g = 149;
	col.b = 237;
	window->clear(col);
}

void Game::endDraw()
{
	window->display();
}

// check AABB
bool Game::checkCollision(Sprite* s1, Sprite* s2)
{
	if (s1->getAABB().left + s1->getAABB().width < s2->getAABB().left)
		return false;
	if (s1->getAABB().left > s2->getAABB().left + s2->getAABB().width)
		return false;
	if (s1->getAABB().top + s1->getAABB().height < s2->getAABB().top)
		return false;
	if (s1->getAABB().top > s2->getAABB().top + s2->getAABB().height)
		return false;

	return true;
}

// check Sphere bounding collision
bool Game::checkSphereBounding(Sprite* s1, Sprite* s2)
{
	// half width, height, give us the centre of the shape
	if (pow(s2->getPosition().x - s1->getPosition().x, 2) + pow(s2->getPosition().y - s1->getPosition().y, 2) < pow((s2->getSize().x / 2) + (s1->getSize().x / 2), 2))
	{
		return true;
	}
	return false;
}
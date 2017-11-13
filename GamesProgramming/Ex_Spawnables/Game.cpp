#include "Game.h"

Game::Game(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;
	
	texture.loadFromFile("gfx/Mushroom.png");

	player.setSize(sf::Vector2f(100, 100));
	player.setTexture(&texture);
	player.setPosition(100, 100);



	player.setInput(input);
	player.setVelocity(100, 0);

	/*enemyTexture.loadFromFile("gfx/Goomba.png");
	enemy.setSize(sf::Vector2f(50, 50));
	enemy.setTexture(&enemyTexture);
	enemy.setPosition(0, 200);

	cursorTexture.loadFromFile("gfx/icon.png");
	cursor.setTexture(&cursorTexture);
	cursor.setSize(sf::Vector2f(25, 25));
	cursor.setInput(input);*/

	//window->setMouseCursorVisible(false);




	zombTexture.loadFromFile("gfx/animZombie.png");
	zombie.setSize(sf::Vector2f(55, 108));
	zombie.setPosition(200, 200);
	zombie.setTexture(&zombTexture);
	zombie.setFrameSize(55, 108);
	zombie.setAnimationSpeed(0.1);




	/*marioTexture.loadFromFile("gfx/MarioSheetT.png");
	mario.setSize(sf::Vector2f(64, 80));
	mario.setPosition(300, 300);
	mario.setTexture(&marioTexture);
	mario.setInput(input);*/

	ballTexture.loadFromFile("gfx/Beach_Ball.png");
	ball.setSize(sf::Vector2f(100, 100));
	ball.setPosition(500, 50);
	ball.setTexture(&ballTexture);

	//ballTexture.loadFromFile("gfx/Beach_Ball.png");
	ball2.setSize(sf::Vector2f(100, 100));
	ball2.setPosition(200, 50);
	ball2.setTexture(&ballTexture);
	ball2.setInput(input);

	ball3.setSize(sf::Vector2f(100, 100));
	ball3.setPosition(0, 300);
	ball3.setTexture(&ballTexture);
	ball3.setInput(input);

	ball4.setSize(sf::Vector2f(100, 100));
	ball4.setPosition(0, 300);
	ball4.setTexture(&ballTexture);
	ball4.updateAABB();
	//ball4.setInput(input);

	paddle.setSize(sf::Vector2f(300, 50));
	paddle.setPosition(300, 450);
	//paddle.setTexture(&ballTexture);
	paddle.setFillColor(sf::Color::Red);
	paddle.setInput(input);
	paddle.updateAABB();

	// bouncing sqaures
	square1.setTexture(&ballTexture);
	square2.setTexture(&ballTexture);
	square2.setPosition(750, 240);
	square2.setFillColor(sf::Color::Green);
	square2.updateAABB();

	// spawnables
	


}

Game::~Game()
{

}

void Game::update(float dt)
{
	//enemy.update(dt);
	//cursor.update(dt);

	//zombie.update(dt);
	//mario.update(dt);

	//ball.update(dt);
	//ball2.update(dt);
	//ball3.update(dt);

	//ball4.update(dt);
	//paddle.update(dt);

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

	bbManager.update(dt);
}

void Game::handleInput(float dt)
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.

	player.update(dt);

	if (input->isKeyDown(sf::Keyboard::Space))
	{
		input->setKeyUp(sf::Keyboard::Space);
		bbManager.spawn();
	}
}


void Game::render()
{
	beginDraw();
	
	//window->draw(player);
	//window->draw(enemy);
	//window->draw(cursor);

	//window->draw(zombie);
	//window->draw(mario);
	//window->draw(ball);
	//window->draw(ball2);
	//window->draw(ball3);

	//window->draw(ball4);
	//window->draw(paddle);
	//window->draw(square1);
//	window->draw(square2);

	bbManager.render(window);

	endDraw();
}

void Game::beginDraw()
{
	window->clear(sf::Color::Black);
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
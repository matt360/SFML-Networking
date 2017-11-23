#include "GameClientState.h"

GameClientState::GameClientState(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	lerp_mode = true;
	debug_mode = false;
	debug_message = false;

	// 1 - for linear interpolation, 2 - for quadratic interpolation
	num_messages = 1;

	font.loadFromFile("font/advanced_pixel-7.ttf");
	text.setFont(font);
	text.setCharacterSize(18);
	text.setString("hellow world");

	texture.loadFromFile("gfx/MushroomTrans.png");

	player.setSize(sf::Vector2f(32, 32));
	player.setTexture(&texture);
	sf::Vector2f initial_player_position(5.0f, 5.0f);
	player.setPosition(initial_player_position);
	PlayerMessage initial_player_message;
	initial_player_message.position.x = initial_player_position.x;
	initial_player_message.position.y = initial_player_position.y;
	local_positions.push_front(initial_player_message);
	local_positions.push_back(initial_player_message);
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


	audioMgr.addMusic("sfx/cantina.wav", "cantina");
	audioMgr.addMusic("sfx/hyrulefield.wav", "hyrule");

	audioMgr.addSound("sfx/SMB_jump-small.wav", "jump");
	audioMgr.addSound("sfx/SMB_1-up.wav", "up");
	audioMgr.addSound("sfx/getover.wav", "getover");
	audioMgr.addSound("sfx/TP_Secret.wav", "secret");
	audioMgr.addSound("sfx/Glass_Break.wav", "glass");

	hasStarted = false;
	//audioMgr.playSoundbyName("cantina");

	//	int err = buff.loadFromFile("sfx/cantina.ogg");
	//soun.setBuffer(buff);
	//soun.play();
	//sf::Music music;
}

GameClientState::~GameClientState()
{

}

void GameClientState::render()
{
	beginDraw();

	level.render(window);
	window->draw(player);
	window->draw(text);

	endDraw();
}

void GameClientState::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void GameClientState::endDraw()
{
	window->display();
}

void GameClientState::handleInput()
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.
	
	// toggle debug mode to display socket messages
	if (input->isKeyDown(sf::Keyboard::D))
	{
		input->setKeyUp(sf::Keyboard::D);
		debug_mode = !debug_mode;
	}
	// toggle debug messages to display messages
	if (input->isKeyDown(sf::Keyboard::M))
	{
		input->setKeyUp(sf::Keyboard::M);
		debug_message = !debug_message;
	}
	// toggle lerp mode
	if (input->isKeyDown(sf::Keyboard::L))
	{
		input->setKeyUp(sf::Keyboard::L);
		lerp_mode = !lerp_mode;
		// display lerp_mode state
		lerp_mode ? std::cout << "Lerp is ON\n" : std::cout << "Lerp is OFF. Using the network path directly\n";

	}
}

void GameClientState::update()
{
	//std::thread st1(call_once_set_window, *window);
	//st1.join();
	//call_once_set_window(sf::Vector2i(1200, 1000));

	//fps = 1.f / dt;
	//text.setString(std::to_string(fps));
	if (fps > 60) fps = 0;

	if (!hasStarted)
	{
		audioMgr.playMusicbyName("cantina");
		hasStarted = true;
	}

	// TODO keep track of local positions
	keepTrackOfLocalPositoins(player, clock, offset);

	/*if (input->isKeyDown(sf::Keyboard::Up))
	{
		input->setKeyUp(sf::Keyboard::Up);
		player.jump();
		audioMgr.playSoundbyName("jump");
	}*/

	//player.update(dt);

	if (input->isKeyDown(sf::Keyboard::Num1))
	{
		input->setKeyUp(sf::Keyboard::Num1);
		audioMgr.playSoundbyName("up");
	}

	if (input->isKeyDown(sf::Keyboard::Num2))
	{
		input->setKeyUp(sf::Keyboard::Num2);
		audioMgr.playSoundbyName("getover");
	}
	if (input->isKeyDown(sf::Keyboard::Num3))
	{
		input->setKeyUp(sf::Keyboard::Num3);
		audioMgr.playSoundbyName("glass");
	}
	if (input->isKeyDown(sf::Keyboard::BackSpace))
	{
		input->setKeyUp(sf::Keyboard::BackSpace);
		audioMgr.stopAllMusic();
	}
	if (input->isKeyDown(sf::Keyboard::Num4))
	{
		input->setKeyUp(sf::Keyboard::Num4);
		audioMgr.playMusicbyName("hyrule");
	}

	// send packets at 10Hz rate (at 10PFS)
	//if ((int)fps % 6 == 0)
	// send packets at 30Hz rate (at 30PFS)
	//if ((int)fps % 2 == 0)
		sendPacket(player, clock, offset, debug_mode);

	checkForIncomingPackets(debug_mode);

	// FIXME: Implement prediction here!
	// You have:
	// - the history of position messages received, in "messages_"
	//   (msg0 is the most recent, msg1 the 2nd most recent, msg2 the 3rd most recent)
	// - the current time, in "time"
	// You need to update:
	// - the predicted position at the current time, in "x_" and "y_"
	if (debug_message) std::cout << "function call: getCurrentTime(): " << getCurrentTime(clock, offset) << "\n";

	switch (network_positions.size())
	{
		case 2:
			linearInterpolation(player, clock, offset, lerp_mode);
		break;

		case 3:
			quadraticInterpolation(player, clock, offset, lerp_mode);
		break;
	}

	// increase fps
	fps++;
}
#include "GameClientState.h"

GameClientState::GameClientState(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	lerp_mode = false;
	debug_mode = false;
	debug_message = false;

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
	//window->draw(text);

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

inline float GameClientState::lerp(float start, float end, float time)
{
	return start * (1.0f - time) + time * end;
}

sf::Vector2f GameClientState::lerp(const sf::Vector2f& start, const sf::Vector2f& end, const float time)
{
	sf::Vector2f temp;
	temp.x = lerp(start.x, end.x, time);
	temp.y  = lerp(start.y, end.y, time);
	return temp;
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
		sendPacket(player, clock, offset);

	checkForIncomingPackets();

	// FIXME: Implement prediction here!
	// You have:
	// - the history of position messages received, in "messages_"
	//   (msg0 is the most recent, msg1 the 2nd most recent, msg2 the 3rd most recent)
	// - the current time, in "time"
	// You need to update:
	// - the predicted position at the current time, in "x_" and "y_"
	if (debug_message) std::cout << "function call: getCurrentTime(): " << getCurrentTime(clock, offset) << "\n";

	if (network_positions.size() == 2)
	{
		sf::Vector2f local_path = predict_local_path(clock, offset);
		sf::Vector2f network_path = predict_network_path(clock, offset);
		//lerp
		sf::Vector2f lerp_position = lerp(local_path, network_path, 0.1f);
		//sf::Vector2f lerp_position = lerp(local_path, network_path, 1.0f);
		// set position
		if (lerp_mode) player.setPosition(lerp_position);
		else player.setPosition(network_path);

		// add lerped to the history of the local posistions
		keepTrackOfLocalPositoins(lerp_position, clock, offset);
	}
	if (network_positions.size() == 3) 
	{
		// quadratic model
		float
			x_average_velocity_1,
			y_average_velocity_1,
			x_average_velocity_2,
			y_average_velocity_2,
			a_x,
			a_y,
			x_,
			y_;

		PlayerMessage msg0 = network_positions.at(0);
		PlayerMessage msg1 = network_positions.at(1);
		PlayerMessage msg2 = network_positions.at(2);
		float time = (float)getCurrentTime(clock, offset);

		// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
		x_average_velocity_1 = (msg0.position.x - msg1.position.x) / (msg0.time - msg1.time);
		y_average_velocity_1 = (msg0.position.y - msg1.position.y) / (msg0.time - msg1.time);

		x_average_velocity_2 = (msg1.position.x - msg2.position.x) / (msg1.time - msg2.time);
		y_average_velocity_2 = (msg1.position.y - msg2.position.y) / (msg1.time - msg2.time);

		a_x = (x_average_velocity_2 - x_average_velocity_1);
		a_y = (y_average_velocity_2 - y_average_velocity_1);

		// s = s0 + v0t + ½at2
		x_ = msg2.position.x + (x_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_x) * powf((time - msg2.time), 2));
		y_ = msg2.position.y + (y_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_y) * powf((time - msg2.time), 2));

		sf::Vector2f loc_player_pos(x_, y_);
		player.setPosition(loc_player_pos);
	}

	// increase fps
	fps++;
}
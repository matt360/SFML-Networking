#include "Game.h"

void main(int argc, void** argv[])
{
	// Program entry point

	// Create game object
	Game game;

	while (!game.getWindow()->isDone())
	{
		// Game loop
		game.handleInput();
		game.update();
		game.render();
	}
}
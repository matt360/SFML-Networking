//#include <SFML/Graphics.hpp>
#include "Game.h"

void main(int argc, char** argv[])
{
	
	sf::RenderWindow window(sf::VideoMode(800, 600), "Lab 1");// , sf::Style::None);
	//window.setPosition(sf::Vector2i(250, 250));
	//window.setSize(sf::Vector2u(400, 400));

	Game game(&window);


	//sf::Texture mushroomTexture;
	//mushroomTexture.loadFromFile("Mushroom.png");
	////mushroomTexture.loadFromFile("sfml-logo-small.png");
	//sf::Sprite mushroom(mushroomTexture);
	//sf::Vector2u size = mushroomTexture.getSize();
	//mushroom.setOrigin(size.x / 2, size.y / 2);
	//mushroom.setPosition(50, 50);
	//sf::Vector2f increment(0.1f, 0.1f);

	sf::Clock clock;
	float deltaTime;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::Resized)
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			break;
		}

		// Calculate delta time. How much time has passed since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();

		// Call standard game loop functions (input, update and render)
		game.handleInput(deltaTime);
		game.update(deltaTime);
		game.render();

		//if ((mushroom.getPosition().x + (size.x / 2) > window.getSize().x && increment.x > 0 || (mushroom.getPosition().x - (size.x / 2)) < 0 && increment.x < 0))
		//{
		//	// Reverse the direction on X axis
		//	increment.x = -increment.x;
		//}
		//if ((mushroom.getPosition().y + (size.y / 2) > window.getSize().y && increment.y > 0 || (mushroom.getPosition().y - (size.y / 2)) < 0 && increment.y < 0))
		//{
		//	// Reverse the direction on Y axis
		//	increment.y = -increment.y;
		//}

		//mushroom.setPosition(mushroom.getPosition() + increment);

		//window.clear(sf::Color(16, 16, 16, 255)); // set background colour to dark grey.
		//window.draw(mushroom);	// render mushroom sprite
		//window.display();	// Swap buffers.
	}

}
#include <SFML/Graphics.hpp>
#include "GameBoard.h"

#define GAME_WIDTH 900
#define GAME_HEIGHT 800

int main()
{
	sf::RenderWindow window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Connect 4");

	//FPS
	window.setFramerateLimit(60);

	//Instantiate things here
	GameBoard game(window);

	while (window.isOpen())
	{
		//Update stuff here
		game.Update();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		//Draw stuff to the screen here, after .clear
		game.Draw();

		window.display();
	}

	return 0;
}
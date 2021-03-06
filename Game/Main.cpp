#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameBoard.h"

//Game Window Dimensions
#define GAME_WIDTH 900
#define GAME_HEIGHT 800

int main()
{
	sf::RenderWindow window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Connect 4", sf::Style::Titlebar | sf::Style::Close);
	NetworkConnection mConnection;		//Connection to the server
	
	//FPS
	window.setFramerateLimit(60);

	//Setup game class 
	Game mGame(window, mConnection);

	while (window.isOpen())
	{
		//Update stuff here
		mGame.Update();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mConnection.CloseConnection();		//Disconnect from server before quitting to alert server of disconnect
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonReleased)
				mGame.MouseReleased(event);
			else if (event.type == sf::Event::TextEntered)
				mGame.KeyPressed(event);

		}

		window.clear();

		mGame.Draw();

		window.display();
	}

	return 0;
}
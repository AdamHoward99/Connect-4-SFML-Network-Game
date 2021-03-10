#pragma once
#include "SFML/Graphics.hpp"
#include "GameBoard.h"
#include <cassert>

class Game
{
public:
	Game(sf::RenderWindow& mApp);		//Default constructor
	~Game();		//Default destructor

	void Initialize();
	void Update();
	void Draw();

private:
	sf::RenderWindow& window;

	GameBoard board;		//put into the playstate class

	enum States
	{
		Start_Menu,
		Pause_Menu,
		Win_Menu,
		Play,
		Leaderboard
	};

	enum Turn		//Put into the playstate class
	{
		Player_1_Turn,
		Player_2_Turn
	};

	States mStates = States::Start_Menu;
	Turn mGameTurn = Turn::Player_1_Turn;
};
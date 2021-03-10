#pragma once
#include "SFML/Graphics.hpp"
#include "PlayState.h"
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

	enum States
	{
		Start_Menu,
		Pause_Menu,
		Win_Menu,
		Play,
		Leaderboard
	};

	PlayState mPlayState;

	States mStates = States::Start_Menu;
};
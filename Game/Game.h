#pragma once
#include "SFML/Graphics.hpp"
#include "States.h"
#include "PlayState.h"
#include "Menu.h"
#include "StartMenu.h"
#include "ControlMenu.h"
#include "WinMenu.h"
#include "LeaderboardMenu.h"
#include "PauseMenu.h"
#include <cassert>

class Game
{
public:
	Game(sf::RenderWindow& mApp);		//Default constructor
	~Game();		//Default destructor

	void Initialize();
	void Update();
	void Draw();

	void MouseReleased(sf::Event ev);
	void KeyPressed(sf::Event ev);

private:
	sf::RenderWindow& window;

	void ChangeState(States newState);

	PlayState mPlayState;
	//Could put all menus into a vector of menus?
	StartMenu mStart;
	ControlMenu mControls;
	WinMenu mWin;
	LeaderboardMenu mLeaderboard;
	PauseMenu mPause;

	States mStates = States::Start_Menu;

	std::string winMessage;		//Changes based on if you won, lost or tied
};
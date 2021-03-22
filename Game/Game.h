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
#include "EnterNameMenu.h"
#include "NetworkConnection.h"

#include <cassert>
#include <chrono>
#include <unordered_map>

#define PAUSE_KEY 112

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

	void StartPauseTimer() {mPauseTimer.first = std::chrono::steady_clock::now();}
	void UpdatePauseTimer();

	PlayState mPlayState;

	std::unordered_map<std::string, std::unique_ptr<Menu>> mMenus;

	WinMenu mWin;
	PauseMenu mPause;

	States mStates = States::Start_Menu;

	std::string winMessage;		//Changes based on if you won, lost or tied

	std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point> mPauseTimer;
	float pauseTimerAllowance = 30.f;	//How many secs can they pause for, is reduced every time they hit the limit

	//Show timer
	sf::Text mPauseTimerTxt;
	sf::Font mTextFont;

	//Network Connection
	NetworkConnection mConnection;

	void InitializeMatchmakingScreen();
	void DrawMatchmakingScreen();

	sf::Texture mBackgroundTex;
	sf::Sprite mBackgroundSpr;

	sf::Font mFont;
	std::vector<sf::Text> mLoadingText;

	//Username of player
	sf::String mPlayerName;
};
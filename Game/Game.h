#pragma once
#include "PlayState.h"
#include "StartMenu.h"
#include "ControlMenu.h"
#include "WinMenu.h"
#include "PauseMenu.h"
#include "EnterNameMenu.h"
#include "DisconnectMenu.h"
#include "NetworkConnection.h"

#include <unordered_map>

#define PAUSE_KEY 112

class Game
{
public:
	Game(sf::RenderWindow& mApp, NetworkConnection& mConnection);		//Default constructor
	~Game();															//Default destructor

	void Initialize();
	void Update();
	void Draw();

	void MouseReleased(sf::Event ev);
	void KeyPressed(sf::Event ev);

private:
	sf::RenderWindow& mWindow;
	NetworkConnection& mConnection;

	void ChangeState(States newState);		//Allows other menus to change state of the game

	//Pause Timer Functions
	void StartPauseTimer();
	void UpdatePauseTimer();

	//Matchmaking State Functions
	void InitializeMatchmakingScreen();
	void DrawMatchmakingScreen();

	PlayState mPlayState;
	std::unordered_map<std::string, std::unique_ptr<Menu>> mMenus;	//Stores all menus
	States mStates = States::Start_Menu;

	//Pause Timer Variables
	std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point> mPauseTimer;		//First is when the timer starts, second is current time
	float mPauseTimerAllowance = 30.f;	//How many secs can they pause for, is reduced every time they hit the limit

	//Show timer
	sf::Text mPauseTimerTxt;
	sf::Font mTextFont;

	//Background Variables
	sf::Texture mBackgroundTex;
	sf::Sprite mBackgroundSpr;

	sf::Text mLoadingText;

	//Username of player
	sf::String mPlayerName;
};
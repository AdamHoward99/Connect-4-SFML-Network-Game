#include "Game.h"

Game::Game(sf::RenderWindow& w)
	:window(w), mPlayState(w), mWin(w), mPause(w)
{
	mMenus["StartMenu"] = std::make_unique<StartMenu>(w);
	mMenus["ControlMenu"] = std::make_unique<ControlMenu>(w);
	mMenus["LeaderboardMenu"] = std::make_unique<LeaderboardMenu>(w);


	//Setup elements of game
	Initialize();
}

Game::~Game()
{
	//Remove all pointers
	mMenus["StartMenu"].reset();
	mMenus["ControlMenu"].reset();
	mMenus["LeaderboardMenu"].reset();

}

void Game::Initialize()
{
	//Setup all elements of the game, not needed as constructors of each call initialize
	mPlayState.Initialize();
}

void Game::Update()
{
	//Update each element of the game based on which state is active
	switch (mStates)
	{
	case States::Play:
		mPlayState.Update();

		if (mPlayState.GetIfGameWon())
		{
			mStates = States::Win_Menu;
			mWin.SetWinScreenTitle(mPlayState.GetWinMessage());
			mPlayState.Reset();
		}
		break;

	case States::Pause_Menu:
		mPause.Update();
		UpdatePauseTimer();
		break;

	case States::Control_Menu:
		mMenus["ControlMenu"].get()->Update();
		break;

	case States::Start_Menu:
		mMenus["StartMenu"].get()->Update();
		break;

	case States::Leaderboard:
		mMenus["LeaderboardMenu"].get()->Update();
		break;

	case States::Win_Menu:
		mWin.Update();
		break;

	case States::Quit:
		window.close();
		break;

	default:
		assert(mStates);
		break;
	}

}

void Game::UpdatePauseTimer()
{
	mPauseTimer.second = std::chrono::steady_clock::now();

	if (std::chrono::duration_cast<std::chrono::microseconds>(mPauseTimer.second - mPauseTimer.first).count() / 1000000.f > pauseTimerAllowance)
	{
		mStates = States::Play;
		if(pauseTimerAllowance > 10)	
			pauseTimerAllowance *= 0.5f;
	}
}

void Game::Draw()
{
	//Draw elements of the game based on which state is active
	//Include switch like in update for each state
	switch (mStates)
	{
	case States::Play:
		mPlayState.Draw();
		break;

	case States::Pause_Menu:
		mPlayState.Draw();
		mPause.Draw();
		break;

	case States::Control_Menu:
		mMenus["ControlMenu"].get()->Draw();
		break;

	case States::Start_Menu:
		mMenus["StartMenu"].get()->Draw();
		break;

	case States::Leaderboard:
		mMenus["LeaderboardMenu"].get()->Draw();
		break;

	case States::Win_Menu:
		mWin.Draw();
		break;

	default:
		assert(mStates);
		break;
	}
}

void Game::MouseReleased(sf::Event ev)
{
	if (ev.mouseButton.button == sf::Mouse::Left)
	{
		switch (mStates)
		{
		case States::Play:
			mPlayState.PlacePiece();
			break;

		case States::Pause_Menu:
			ChangeState(mPause.DetectButtonPress());
			if (mPause.GetIfForfeiting())		//Reset game when quitting out from pause menu
				mPlayState.Reset();
			break;

		case States::Control_Menu:
			ChangeState(mMenus["ControlMenu"].get()->DetectButtonPress());
			break;

		case States::Start_Menu:
			ChangeState(mMenus["StartMenu"].get()->DetectButtonPress());
			break;

		case States::Leaderboard:
			ChangeState(mMenus["LeaderboardMenu"].get()->DetectButtonPress());
			break;

		case States::Win_Menu:
			ChangeState(mWin.DetectButtonPress());
			break;

		default:
			assert(mStates);
			break;
		}
		
	}
}

void Game::KeyPressed(sf::Event ev)
{
	if (ev.key.code == sf::Keyboard::P)
	{
		switch (mStates)
		{
		case States::Play:
			mStates = States::Pause_Menu;
			StartPauseTimer();
			break;

		default:
			break;
		}
	}
}

void Game::ChangeState(States newState)
{
	mStates = newState;
}
#include "Game.h"

Game::Game(sf::RenderWindow& w)
	:window(w), mPlayState(w), mStart(w), mControls(w), mWin(w), mLeaderboard(w), mPause(w)
{
	//Setup elements of game
	Initialize();
}

Game::~Game()
{
	//Remove all pointers
}

void Game::Initialize()
{
	//Setup all elements of the game, not needed as constructors of each call initialize
	mPlayState.Initialize();
	mStart.Initialize();
	mControls.Initialize();
	mWin.Initialize();
	//mLeaderboard.Initialize();
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
			mPlayState.Reset();
		}
		break;

	case States::Pause_Menu:
		mPause.Update();
		break;

	case States::Control_Menu:
		mControls.Update();
		break;

	case States::Start_Menu:
		mStart.Update();
		break;

	case States::Leaderboard:
		mLeaderboard.Update();
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
		mControls.Draw();
		break;

	case States::Start_Menu:
		//Start functionality here
		mStart.Draw();
		break;

	case States::Leaderboard:
		mLeaderboard.Draw();
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
			ChangeState(mControls.DetectButtonPress());
			break;

		case States::Start_Menu:
			ChangeState(mStart.DetectButtonPress());
			break;

		case States::Leaderboard:
			ChangeState(mLeaderboard.DetectButtonPress());
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
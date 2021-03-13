#include "Game.h"

Game::Game(sf::RenderWindow& w)
	:window(w), mPlayState(w), mStart(w), mControls(w)
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
	//Setup all elements of the game
	mPlayState.Initialize();
	mStart.Initialize();
	mControls.Initialize();
}

void Game::Update()
{
	//Update each element of the game based on which state is active
	switch (mStates)
	{
	case States::Play:
		mPlayState.Update();
		break;

	case States::Pause_Menu:
		//Pause functionality here
		break;

	case States::Control_Menu:
		mControls.Update();
		break;

	case States::Start_Menu:
		//Start functionality here
		mStart.Update();
		break;

	case States::Leaderboard:
		//Leaderboard functionality here
		break;

	case States::Win_Menu:
		//Win menu functionality here
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
		//Pause functionality here
		break;

	case States::Control_Menu:
		mControls.Draw();
		break;

	case States::Start_Menu:
		//Start functionality here
		mStart.Draw();
		break;

	case States::Leaderboard:
		//Leaderboard functionality here
		break;

	case States::Win_Menu:
		//Win menu functionality here
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
			//Pause functionality here
			break;

		case States::Control_Menu:
			ChangeState(mControls.DetectButtonPress());
			break;

		case States::Start_Menu:
			ChangeState(mStart.DetectButtonPress());
			break;

		case States::Leaderboard:
			//Leaderboard functionality here
			break;

		case States::Win_Menu:
			//Win menu functionality here
			break;

		default:
			assert(mStates);
			break;
		}
		
	}
}

void Game::ChangeState(States newState)
{
	mStates = newState;
}
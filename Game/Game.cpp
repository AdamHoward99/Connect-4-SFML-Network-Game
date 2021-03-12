#include "Game.h"

Game::Game(sf::RenderWindow& w)
	:window(w), mPlayState(w), m(w)
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
	m.Initialize();
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

	case States::Start_Menu:
		//Start functionality here
		m.Update();
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

void Game::Draw()
{
	//Draw elements of the game based on which state is active
	//Include switch like in update for each state
	mPlayState.Draw();
}

void Game::MouseReleased(sf::Event ev)
{
	if (ev.mouseButton.button == sf::Mouse::Left)
	{
		//Put a switch for each state
		mPlayState.PlacePiece();
	}
}
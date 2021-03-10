#include "Game.h"

Game::Game(sf::RenderWindow& w)
	:window(w), board(w)
{
	//Setup elements of game
	Initialize();
}

Game::~Game()
{
	//Remove all pointers
	board.~GameBoard();
}

void Game::Initialize()
{
	//Setup all elements of the game
	board.Initialize();		//Sets up the game board
}

void Game::Update()
{
	//Update each element of the game based on which state is active
	switch (mStates)
	{
	case States::Play:
		board.Update();
		break;

	case States::Pause_Menu:
		//Pause functionality here
		break;

	case States::Start_Menu:
		//Start functionality here
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
	board.Draw();

}
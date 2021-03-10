#include "PlayState.h"

PlayState::PlayState(sf::RenderWindow& mApp)
	:window(mApp), board(mApp)
{

}

PlayState::~PlayState()
{
	//Destructor
	board.~GameBoard();
}

void PlayState::Initialize()
{
	board.Initialize();
}

void PlayState::Update()
{
	board.Update();
}

void PlayState::Draw()
{
	board.Draw();
}
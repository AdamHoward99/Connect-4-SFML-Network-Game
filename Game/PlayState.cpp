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

	pieceToAdd = sf::CircleShape(30.f);

	if (mGameTurn == Turn::Player_1_Turn)
		pieceToAdd.setFillColor(sf::Color::Red);
	else
		pieceToAdd.setFillColor(sf::Color::Yellow);

}

void PlayState::Update()
{
	board.Update();

	UpdateMousePosition();

	//Switch turns, put into its own function
	if (turnEnd)
		SwitchTurns();		//Changes piece colour and turn variable, sets turnEnd to false

}

void PlayState::UpdateMousePosition()
{
	//Updating mouse position
	mousePos = sf::Mouse::getPosition(window);

	//Todo: could make this look better
	if (mousePos.x <= 100)
		mousePos.x = 50;
	else if (mousePos.x <= 200)
		mousePos.x = 150;
	else if (mousePos.x <= 300)
		mousePos.x = 250;
	else if (mousePos.x <= 400)
		mousePos.x = 350;
	else if (mousePos.x <= 500)
		mousePos.x = 450;
	else if (mousePos.x <= 600)
		mousePos.x = 550;
	else if (mousePos.x <= 700)
		mousePos.x = 650;
	else
		mousePos.x = 650;

	pieceToAdd.setPosition(static_cast<float>(mousePos.x), 20.f);
}

void PlayState::SwitchTurns()
{
	if (mGameTurn == Turn::Player_1_Turn)
	{
		mGameTurn = Turn::Player_2_Turn;
		pieceToAdd.setFillColor(sf::Color::Yellow);
	}
	else
	{
		mGameTurn = Turn::Player_1_Turn;
		pieceToAdd.setFillColor(sf::Color::Red);
	}

	turnEnd = false;
}

void PlayState::PlacePiece()
{
	int col;
	if (mousePos.x == 50.f)
		col = 0;
	else if (mousePos.x == 150.f)
		col = 1;
	else if (mousePos.x == 250.f)
		col = 2;
	else if (mousePos.x == 350.f)
		col = 3;
	else if (mousePos.x == 450.f)
		col = 4;
	else if (mousePos.x == 550.f)
		col = 5;
	else if (mousePos.x == 650.f)
		col = 6;

	for (int i = BOARD_HEIGHT; i >= 0; i--)
	{
		if (board.pieces[i][col].getFillColor() == sf::Color::White)		//Finds an empty piece
		{
			//Add stuff for when the column is full
			pieceToAdd.setPosition(board.pieces[i][col].getPosition());
			board.pieces[i][col] = pieceToAdd;
			turnEnd = true;
			break;
		}
	}
}

void PlayState::Draw()
{
	board.Draw();
	window.draw(pieceToAdd);
}
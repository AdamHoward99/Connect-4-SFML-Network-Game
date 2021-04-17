#include "GameBoard.h"

GameBoard::GameBoard(sf::RenderWindow& mApp)
	:window(mApp)
{}

GameBoard::~GameBoard()
{

}

void GameBoard::Initialize()
{
	//Starting positions for the pieces, changes in for loop
	float xPosition = 50.f;
	float yPosition = 150.f;

	//Default piece to fill the board up
	sf::CircleShape mPiece = sf::CircleShape(30.f);
	mPiece.setFillColor(sf::Color::White);
	mPiece.setOutlineThickness(5);
	mPiece.setOutlineColor(sf::Color::Color(70, 96, 239));

	//Loops through adding empty pieces to set up the game board
	for (int x = 1; x < BOARD_HEIGHT; x++)		
	{
		for (int y = 1; y < BOARD_WIDTH; y++)
		{
			mPiece.setPosition(xPosition, yPosition);
			xPosition += mPieceOffset;
			pieces[x][y] = mPiece;
		}
		xPosition = 50.f;
		yPosition += mPieceOffset;
	}

	//Background of the game board
	mBoardBackground.setSize(sf::Vector2f(700.f, 600.f));
	mBoardBackground.setPosition(30.f, 130.f);
	mBoardBackground.setFillColor(mBoardColour);

	//Background
	if (!mBackgroundTex.loadFromFile("bin/Textures/menuBackground.png"))
		assert(!mBackgroundTex.loadFromFile("bin/Textures/menuBackground.png"));

	mBackgroundTex.setSmooth(true);

	mBackgroundSpr.setTexture(mBackgroundTex);
	mBackgroundSpr.setPosition(0.f, 0.f);
}

void GameBoard::Update()
{

}

void GameBoard::Draw()
{
	window.draw(mBackgroundSpr);
	window.draw(mBoardBackground);

	//Displays all pieces in the game board
	for (int x = 1; x < BOARD_HEIGHT; x++)
		for (int y = 1; y < BOARD_WIDTH; y++)
			window.draw(pieces[x][y]);
}

void GameBoard::ResetBoard()
{
	//Changes all pieces back to default pieces for next game
	for (int x = 1; x < BOARD_HEIGHT; x++)
		for (int y = 1; y < BOARD_WIDTH; y++)
		{
			pieces[x][y].setFillColor(sf::Color::White);
			pieces[x][y].setOutlineColor(sf::Color::Color(70, 96, 239));
			pieces[x][y].setOutlineThickness(5);
		}
}

bool GameBoard::CheckIfBoardIsFull()
{
	//Checks if all top slots are filled
	for (int y = 1; y < BOARD_WIDTH; y++)
	{
		if (pieces[1][y].getFillColor() == sf::Color::White)		//A piece is still available in the board	
		{
			return false;		//The board is not full
		}
	}

	return true;		//The board is full
}

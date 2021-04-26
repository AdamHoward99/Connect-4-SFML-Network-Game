#include "GameBoard.h"

GameBoard::GameBoard(sf::RenderWindow& mApp)
	:mWindow(mApp)
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
			mBoard[x][y] = mPiece;
		}
		xPosition = 50.f;
		yPosition += mPieceOffset;
	}

	//Background of the game board
	mBoardBackground.setSize(sf::Vector2f(700.f, 600.f));
	mBoardBackground.setPosition(30.f, 130.f);
	mBoardBackground.setFillColor(mBoardColour);

	//Background
	mBackgroundTex.loadFromFile("../bin/Textures/menuBackground.png");
	mBackgroundTex.setSmooth(true);

	mBackgroundSpr.setTexture(mBackgroundTex);
	mBackgroundSpr.setPosition(0.f, 0.f);
}

void GameBoard::Update(std::pair<int, int>& mLastMove, const int mPlayerType)
{
	//Update appearance of the game board to reflect other players turn
	if (mLastMove != std::pair<int, int>{-1, -1})
	{
		sf::CircleShape piece = sf::CircleShape(30.f);
		piece.setPosition(mBoard[mLastMove.first][mLastMove.second].getPosition());

		//Finds which player the opponent is to determine which colour the piece will be
		if (mPlayerType == 1)
			piece.setFillColor(mPlayer2PieceColour);
		else
			piece.setFillColor(mPlayer1PieceColour);

		mBoard[mLastMove.first][mLastMove.second] = piece;
		mLastMove = { -1, -1 };		//Made changes to board, reset data
	}
}

void GameBoard::Draw()
{
	mWindow.draw(mBackgroundSpr);
	mWindow.draw(mBoardBackground);

	//Displays all pieces in the game board
	for (int x = 1; x < BOARD_HEIGHT; x++)
		for (int y = 1; y < BOARD_WIDTH; y++)
			mWindow.draw(mBoard[x][y]);
}

void GameBoard::ResetBoard()
{
	//Changes all pieces back to default pieces for next game
	for (int x = 1; x < BOARD_HEIGHT; x++)
		for (int y = 1; y < BOARD_WIDTH; y++)
		{
			mBoard[x][y].setFillColor(sf::Color::White);
			mBoard[x][y].setOutlineColor(sf::Color::Color(70, 96, 239));
			mBoard[x][y].setOutlineThickness(5);
		}
}

bool GameBoard::CheckIfBoardIsFull()
{
	//Checks if all top slots are filled
	for (int y = 1; y < BOARD_WIDTH; y++)
	{
		if (mBoard[1][y].getFillColor() == sf::Color::White)		//A piece is still available in the board	
		{
			return false;		//The board is not full
		}
	}

	return true;		//The board is full
}

bool GameBoard::Connected4(const int mPlayerType, const sf::Vector2i mLastMove)
{
	//Get Colour of piece looking to connect
	sf::Color mPlayerColour;

	if (mPlayerType == 1)
		mPlayerColour = mPlayer1PieceColour;
	else
		mPlayerColour = mPlayer2PieceColour;


	if (HorizontalConnectCheck(mPlayerColour, mLastMove))		//Checks horizontally for a connect 4
		return true;

	if (VerticalConnectCheck(mPlayerColour, mLastMove))			//Checks vertically for a connect 4
		return true;

	if (DiagonalConnectCheck(mPlayerColour, mLastMove))			//Checks diagonally for a connect 4
		return true;

	return false;
}

bool GameBoard::HorizontalConnectCheck(sf::Color mPlayerColour, const sf::Vector2i mLastMove)
{
	int mAmountConnected = 0;		//Amount of pieces of the same colour connected together

	int y = mLastMove.y;
	while (y >= 1 && mBoard[mLastMove.x][y].getFillColor() == mPlayerColour)		//Horizontal Left searching
	{
		y--;
		mAmountConnected++;
		if (mAmountConnected >= 4)
			return true;
	}

	mAmountConnected--;		//Remove as the next will read the most recent piece added

	y = mLastMove.y;
	while (y < BOARD_WIDTH && mBoard[mLastMove.x][y].getFillColor() == mPlayerColour)	//Horizontal Right searching
	{
		y++;
		mAmountConnected++;
		if (mAmountConnected >= 4)
			return true;
	}

	return false;
}

bool GameBoard::VerticalConnectCheck(sf::Color mPlayerColour, const sf::Vector2i mLastMove)
{
	int mAmountConnected = 0;

	int x = mLastMove.x;
	while (x >= 1 && mBoard[x][mLastMove.y].getFillColor() == mPlayerColour)			//Vertical searching
	{
		x++;
		mAmountConnected++;
		if (mAmountConnected >= 4)
			return true;
	}

	return false;
}

bool GameBoard::DiagonalConnectCheck(sf::Color mPlayerColour, const sf::Vector2i mLastMove)
{
	int mAmountConnected = 0;
	int x = mLastMove.x;
	int y = mLastMove.y;

	while (x >= 1 && y < BOARD_WIDTH && mBoard[x][y].getFillColor() == mPlayerColour)		//North-east movement
	{
		x--;
		y++;
		mAmountConnected++;
		if (mAmountConnected >= 4)
			return true;
	}

	mAmountConnected--;
	x = mLastMove.x;
	y = mLastMove.y;
	while (x < BOARD_HEIGHT && y >= 1 && mBoard[x][y].getFillColor() == mPlayerColour)	//South-west movement
	{
		x++;
		y--;
		mAmountConnected++;
		if (mAmountConnected >= 4)
			return true;
	}

	mAmountConnected = 0;
	x = mLastMove.x;
	y = mLastMove.y;
	while (x < BOARD_HEIGHT && y < BOARD_WIDTH && mBoard[x][y].getFillColor() == mPlayerColour)		//South-east movement
	{
		x++;
		y++;
		mAmountConnected++;
		if (mAmountConnected >= 4)
			return true;
	}

	mAmountConnected--;
	x = mLastMove.x;
	y = mLastMove.y;
	while (x >= 1 && y >= 1 && mBoard[x][y].getFillColor() == mPlayerColour)		//North-west searching
	{
		x--;
		y--;
		mAmountConnected++;
		if (mAmountConnected >= 4)
			return true;
	}

	return false;
}

bool GameBoard::PlacePiece(const int mColumn, sf::CircleShape mPiece, sf::Vector2i& mLastMove)
{
	for (int i = BOARD_HEIGHT - 1; i >= 1; i--)
	{
		if (mBoard[i][mColumn].getFillColor() == sf::Color::White)		//Finds an empty piece
		{
			//Add stuff for when the column is full
			mPiece.setPosition(mBoard[i][mColumn].getPosition());
			mBoard[i][mColumn] = mPiece;
			mLastMove = sf::Vector2i(i, mColumn);
			return true;							//Did put a piece in the board
		}
	}

	return false;		//Didn't manage to put a piece in the board
}

bool GameBoard::AutomaticPiecePlace(sf::CircleShape mPiece, sf::Vector2i& mLastMove)
{
	for (int y = 1; y < BOARD_WIDTH; y++)
	{
		for (int i = BOARD_HEIGHT - 1; i >= 1; i--)
		{
			if (mBoard[i][y].getFillColor() == sf::Color::White)		//Finds an empty piece
			{
				//Add stuff for when the column is full
				mPiece.setPosition(mBoard[i][y].getPosition());
				mBoard[i][y] = mPiece;
				mLastMove = sf::Vector2i(i, y);
				return true;		//Did manage to add a piece to the board
			}
		}
	}

	return false;		//Didn't manage to add a piece to the board
}
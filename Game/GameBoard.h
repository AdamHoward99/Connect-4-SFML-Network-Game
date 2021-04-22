#pragma once
#include "SFML/Graphics.hpp"

//Game Board Dimensions
#define BOARD_HEIGHT 7
#define BOARD_WIDTH 8

class GameBoard
{
public:
	GameBoard(sf::RenderWindow& window);		//Default Constructor
	~GameBoard();								//Default Destructor
	
	void Initialize();
	void Update(std::pair<int, int>& mLastMove, const int mPlayer);
	void Draw();
	void ResetBoard();
	bool CheckIfBoardIsFull();		//Connects to PlayState class to see if the board is full after a player places a piece

	bool Connected4(const int mPlayer, const sf::Vector2i mLastMove);							//Checks if 4 pieces of matching colour are in a row, results in player winning
	bool HorizontalConnectCheck(sf::Color mPlayerColour, const sf::Vector2i mLastMove);			//Checks if 4 pieces are in a row horizontally
	bool VerticalConnectCheck(sf::Color mPlayerColour, const sf::Vector2i mLastMove);			//Checks if 4 pieces are in a row vertically
	bool DiagonalConnectCheck(sf::Color mPlayerColour, const sf::Vector2i mLastMove);			//Checks if 4 pieces are in a row diagonally

	bool PlacePiece(const int mPieceColumn, sf::CircleShape mPieceToAdd, sf::Vector2i& mLastMove);	//Places piece in the board if there is a space available
	bool AutomaticPiecePlace(sf::CircleShape mPieceToAdd, sf::Vector2i& mLastMove);					//Places piece in the board when turn timer ends

private:
	sf::RenderWindow& mWindow;
	sf::CircleShape mBoard[BOARD_HEIGHT][BOARD_WIDTH];		//Array containing pieces of the board

	//Piece Colours
	const sf::Color mPlayer1PieceColour = sf::Color::Red;
	const sf::Color mPlayer2PieceColour = sf::Color::Yellow;

	//Menu Colours
	const sf::Color mBoardColour = sf::Color::Color(0, 102, 153);
	const sf::Color mBackgroundColour = sf::Color::Color(170, 196, 239);

	//Background Variables
	sf::RectangleShape mBoardBackground;
	
	sf::Texture mBackgroundTex;
	sf::Sprite mBackgroundSpr;
	
	//Piece Position Variables
	const float mPieceOffset = 100.f;
};
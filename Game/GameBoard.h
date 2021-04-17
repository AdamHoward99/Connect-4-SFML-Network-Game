#pragma once
#include "SFML/Graphics.hpp"
#include <cassert>

#define BOARD_HEIGHT 7
#define BOARD_WIDTH 8

class GameBoard
{
public:
	GameBoard(sf::RenderWindow& window);		//Default Constructor
	~GameBoard();								//Default Destructor
	
	void Initialize();
	void Update();
	void Draw();
	void ResetBoard();
	bool CheckIfBoardIsFull();		//Connects to PlayState class to see if the board is full after a player places a piece

	sf::CircleShape pieces[BOARD_HEIGHT][BOARD_WIDTH];		//make function to get this instead of it being public
private:
	sf::RenderWindow &window;

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
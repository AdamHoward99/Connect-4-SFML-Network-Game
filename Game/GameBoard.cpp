#include "GameBoard.h"

GameBoard::GameBoard(sf::RenderWindow& mApp)
	:window(mApp)
{
	//Set up the colours
}

GameBoard::~GameBoard()
{

}

void GameBoard::Initialize()
{
	float xPosition = 50.f;
	float yPosition = 50.f;
	for (int x = 0; x < BOARD_HEIGHT; x++)
	{
		for (int y = 0; y < BOARD_WIDTH; y++)
		{
			sf::CircleShape pieceSlot = sf::CircleShape(30.f);
			pieceSlot.setFillColor(sf::Color::White);
			pieceSlot.setPosition(xPosition, yPosition);
			pieceSlot.setOutlineThickness(5);
			pieceSlot.setOutlineColor(sf::Color::Color(70, 96, 239));
			xPosition += piece_Offset;
			pieces[x][y] = pieceSlot;
		}
		xPosition = 50.f;
		yPosition += piece_Offset;
	}

	board_Background.setSize(sf::Vector2f(700.f, 600.f));
	board_Background.setPosition(30.f, 30.f);
	board_Background.setFillColor(boardColour);

	background.setSize(sf::Vector2f(900, 800));
	background.setPosition(0.f, 0.f);
	background.setFillColor(background_Colour);
}

void GameBoard::Update()
{

}

void GameBoard::Draw()
{
	window.draw(background);
	window.draw(board_Background);

	for (int x = 0; x < BOARD_HEIGHT; x++)
		for (int y = 0; y < BOARD_WIDTH; y++)
			window.draw(pieces[x][y]);
}

void GameBoard::ResetBoard()
{
	for (int x = 0; x < BOARD_HEIGHT; x++)
		for (int y = 0; y < BOARD_WIDTH; y++)
			pieces[x][y].setFillColor(sf::Color::White);
}
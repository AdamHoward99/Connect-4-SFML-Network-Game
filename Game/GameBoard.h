#pragma once

#include "SFML/Graphics.hpp"
#include <cassert>

#define BOARD_HEIGHT 6
#define BOARD_WIDTH 7

class GameBoard
{
public:
	GameBoard(sf::RenderWindow& window);		//Default Constructor
	~GameBoard();		//Destructor
	
	void Initialize();
	void Update();
	void Draw();

	void ResetBoard();

	sf::CircleShape pieces[BOARD_HEIGHT][BOARD_WIDTH];		//make function to get this instead of it being public
private:

	sf::RenderWindow &window;

	const sf::Color Player1_Piece_Colour = sf::Color::Red;
	const sf::Color Player2_Piece_Colour = sf::Color::Yellow;
	const sf::Color boardColour = sf::Color::Color(0, 102, 153);
	const sf::Color background_Colour = sf::Color::Color(170, 196, 239);

	sf::RectangleShape board_Background;
	
	sf::Texture backgroundTex;
	sf::Sprite backgroundSpr;

	const float piece_Offset = 100.f;




};
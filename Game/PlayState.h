#pragma once

#include "SFML/Graphics.hpp"
#include "GameBoard.h"

class PlayState
{
public:
	PlayState(sf::RenderWindow& window);		//Default Constructor
	~PlayState();

	void Initialize();
	void Update();
	void Draw();

	void PlacePiece();

private:
	sf::RenderWindow& window;

	void UpdateMousePosition();
	void SwitchTurns();

	enum Turn
	{
		Player_1_Turn,
		Player_2_Turn
	};

	Turn mGameTurn = Turn::Player_1_Turn;

	GameBoard board;
	sf::CircleShape pieceToAdd;

	sf::Vector2i mousePos;

	bool turnEnd = false;

};
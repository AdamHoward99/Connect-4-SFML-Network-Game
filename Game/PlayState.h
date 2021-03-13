#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
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

	void SetIfGameWon(bool foo) { gameWon = foo; }
	bool GetIfGameWon() { return gameWon; }

	void Reset();
private:
	sf::RenderWindow& window;

	void UpdateMousePosition();
	void DecideTurnOrder();
	void SwitchTurns();
	bool IsBoardFull();
	bool HasConnected4();


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
	sf::Vector2i lastMove;		//Stores position of last move on board to easier check for a win

	bool gameWon = false;

	std::pair<sf::SoundBuffer, sf::Sound> mPieceSfx;
};
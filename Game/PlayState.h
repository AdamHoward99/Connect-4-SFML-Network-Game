#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameBoard.h"
#include <chrono>

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

	std::string GetWinMessage() { return winMessage; }

	void Reset();

	std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point> mTurnTimer;
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

	std::string winMessage;

	void UpdateTurnTimer();
	void AutomaticPiecePlacement();

	sf::Font mFont;
	sf::Text mTimerText;
};
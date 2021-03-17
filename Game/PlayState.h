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
	void Reset();
	void ButtonPress();		//Detects if over the chat button, if not place's piece

	void SetIfGameWon(bool foo) { gameWon = foo; }
	bool GetIfGameWon() { return gameWon; }

	bool GetIfChatIsOpen() { return isChatOpen; }

	std::string GetWinMessage() { return winMessage; }

	std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point> mTurnTimer;

	void ChatInput(sf::Event ev);

private:
	sf::RenderWindow& window;

	void PlacePiece();
	void UpdateMousePosition();
	void DecideTurnOrder();
	void SwitchTurns();
	bool IsBoardFull();
	bool HasConnected4();

	//Setup functions
	void SetupTextures();
	void SetupSprites();
	void SetupFonts();
	void SetupText();
	void SetupAudio();

	enum Turn
	{
		Player_1_Turn,
		Player_2_Turn
	};

	Turn mGameTurn = Turn::Player_1_Turn;

	GameBoard board;
	sf::CircleShape pieceToAdd;

	sf::Vector2f mousePos;
	float xColumnPosition = 0.f;

	bool gameWon = false;
	bool turnEnd = false;
	sf::Vector2i lastMove;		//Stores position of last move on board to easier check for a win

	std::pair<sf::SoundBuffer, sf::Sound> mPieceSfx;

	std::string winMessage;

	void UpdateTurnTimer();
	void AutomaticPiecePlacement();

	sf::Font mFont;
	sf::Text mTimerText;

	//Chat Variables
	sf::Text mChatButtonText;
	sf::Sprite mChatButton;
	sf::Texture mButtonTex;

	sf::String chatInput;
	sf::Text mChatText;

	//sf::String mChatLog;
	std::vector<sf::String> mChatLog;
	std::vector<sf::Text> mChatLogText;
	//sf::Text mChatLogText;

	sf::Texture mChatPanelTex;
	sf::Sprite mChatPanelSpr;

	bool isChatOpen = false;
};
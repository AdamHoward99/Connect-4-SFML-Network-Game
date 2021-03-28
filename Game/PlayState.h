#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameBoard.h"
#include "NetworkConnection.h"
#include "Turn.h"
#include <chrono>

#define BACKSPACE_KEY 8
#define ENTER_KEY 13

class PlayState
{
public:
	PlayState(sf::RenderWindow& window, NetworkConnection& connection);		//Default Constructor
	~PlayState();

	void Initialize();
	void Update();
	void Draw();
	void Reset();
	void ChatInput(sf::Event ev);

	void ButtonPress();		//Detects if over the chat button, if not, place's piece

	//Game Won Variable Functions
	void SetIfGameWon(bool foo) { gameWon = foo; }
	bool GetIfGameWon() { return gameWon; }

	//Chat Functions
	bool GetIfChatIsOpen() { return isChatOpen; }

	//Win Message Function
	std::string GetWinMessage() { return winMessage; }

	//Turn Timer Variable
	std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point> mTurnTimer;

	//Name Functions
	void SetName(sf::String s) { mName = s; }

	//Set which player in the game via server functions
	int GetPlayer() { return player; }
	void SetPlayer(int p);

	bool IsPlayersTurn();		//Returns if the player can go

private:
	sf::RenderWindow& window;
	NetworkConnection& mServer;

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

	//Chat Log Functions
	void UpdateChatLog();

	//Chat Log Variables
	std::vector<sf::String> mChatLog;
	std::vector<sf::Text> mChatLogText;

	//Turn Variables
	Turn mGameTurn = Turn::Player_1_Turn;
	bool turnEnd = false;

	//Board & Piece Variables
	GameBoard board;
	sf::CircleShape pieceToAdd;
	sf::Vector2f mousePos;
	float xColumnPosition = 0.f;
	sf::Vector2i lastMove;		//Stores position of last move on board to easier check for a win

	bool gameWon = false;

	//Sound Variables
	std::pair<sf::SoundBuffer, sf::Sound> mPieceSfx;

	//Font Variables
	sf::Font mFont;

	//Win Message Variables
	std::string winMessage;

	//Timer Variables
	sf::Text mTimerText;
	void UpdateTurnTimer();
	void AutomaticPiecePlacement();

	//Chat Button Variables
	sf::Text mChatButtonText;
	sf::Sprite mChatButton;
	sf::Texture mButtonTex;

	//Chat Input Variables
	sf::String mChatInput;
	sf::Text mChatInputText;
	const unsigned int ChatLogCharacterLimit = 25;

	//Chat Log Panel Variables
	sf::Texture mChatPanelTex;
	sf::Sprite mChatPanelSpr;
	bool isChatOpen = false;

	//Player Name
	sf::String mName;

	int player = 0;
};
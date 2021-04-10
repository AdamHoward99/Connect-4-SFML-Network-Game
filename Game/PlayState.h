#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameBoard.h"
#include "NetworkConnection.h"
#include "GameData.h"
#include "States.h"
#include <chrono>

#define BACKSPACE_KEY 8
#define ENTER_KEY 13
#define SPACE_KEY 32

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

	GameData& GetData() { return mGameData; }

private:
	sf::RenderWindow& window;
	NetworkConnection& mServer;

	void PlacePiece();
	void UpdateMousePosition();
	bool IsBoardFull();
	bool HasConnected4();

	//Setup functions
	void SetupTextures();
	void SetupSprites();
	void SetupFonts();
	void SetupText();
	void SetupAudio();

	//Server Data Functions
	void BoardUpdateServer();
	void ChatUpdateServer();

	//Chat Log Functions
	void UpdateChatLog();

	//Chat Log Variables
	std::vector<sf::String> mChatLog;
	std::vector<sf::Text> mChatLogText;

	//Turn Variables
	GameData mGameData;
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
	void UpdateTurnTimer();
	void AutomaticPiecePlacement();

	//Chat Button Variables
	sf::Texture mButtonTex;

	//Chat Input Variables
	sf::String mChatInput;
	const unsigned int ChatLogCharacterLimit = 25;

	//Chat Log Panel Variables
	sf::Texture mChatPanelTex;
	bool isChatOpen = false;

	//Player Name
	sf::String mName;

	int player = 0;

	//Text Variables
	std::vector<sf::Text> mText;
	int TextAmount;

	//Sprite Variables
	std::vector<sf::Sprite> mSprites;
	int SpriteAmount;

};
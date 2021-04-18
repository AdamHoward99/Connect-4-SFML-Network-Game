#pragma once
#include "GameBoard.h"
#include "NetworkConnection.h"
#include "GameData.h"
#include "Menu.h"
#include <chrono>

class PlayState
{
public:
	PlayState(sf::RenderWindow& mWindow, NetworkConnection& connection);		//Default Constructor
	~PlayState();																//Default Destructor

	void Initialize();
	void Update();
	void Draw();
	void Reset();
	void ChatInput(sf::Event ev);	//Detects keys entered when chat is open
	void ButtonPress();				//Detects if over the chat button, if not, place's piece

	//Get Functions
	bool ChatOpen() { return mChatOpen; }		//Used to allow 'P' to pause the game when chat is not open
	GameData& GetData() { return mServer.mGameData; }

	//Set Functions
	void SetName(sf::String s) { mName = s; }		//Gets entered name of player from enter name menu
	void SetPlayer(int p);							//Sets player type to either 1 or 2, data is received from the server

	//Turn Timer Variable
	std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point> mTurnTimer;		//First is start time, second is current time

	//Game Check Functions
	bool IsPlayersTurn() { return mServer.mGameData.mTurn == mPlayer; }			//Returns if the player can go
	bool PlayerDisconnected() { return GetData().mDisconnected == -1; }			//Returns true if mDisconnected value == -1, representing opponent disconnect

private:
	sf::RenderWindow& mWindow;
	NetworkConnection& mServer;

	//Game Board Related Functions
	void AddPiece();
	void AutomaticPiecePlacement();
	void TransitionTurn();		//Checks if game can be won, also sends data between players over server
	void SwapPlayerTurn();		//Changes the player turn once the player has made a move

	//Setup functions
	void SetupTextures();
	void SetupSprites();
	void SetupFonts();
	void SetupText();
	void SetupAudio();

	//Mouse Position Functions
	void UpdateMousePosition();

	//Chat Functions
	void UpdateChatLog();
	void ChatUpdateServer();

	//Timer Functions
	void UpdateTurnTimer();

	//Chat Log Variables
	sf::String mChatInput;					//Stores message next to be sent
	std::vector<sf::String> mChatLog;		//Stores all strings of previous chat messages
	std::vector<sf::Text> mChatLogText;		//Used to display on screen all chat messages
	const unsigned int mChatLogCharacterLimit = 20;		//Max characters that can be typed in a single message
	sf::Texture mButtonTex;					//Texture for chat button

	//Chat Log Panel Variables
	sf::Texture mChatPanelTex;
	bool mChatOpen = false;			//Is true when chat button is pressed

	//Turn Variables
	bool mTurnEnd = false;

	//Board & Piece Variables
	GameBoard mGameBoard;			//Stores array of pieces
	sf::CircleShape mPieceToAdd;	//Piece at top of board, will be added to board when player left clicks
	float mColumnPositionX = 0.f;	//Locks added piece position on screen to align with board slots
	sf::Vector2i mLastMove;			//Stores position of last move on board to easier check for a win
	int mPieceColumn = 0;			//Column that next piece will be placed, aligns with column position

	//Sound Variables
	std::pair<sf::SoundBuffer, sf::Sound> mPieceSfx;

	//Font Variables
	sf::Font mFont;

	//On Screen Text Variables
	std::vector<sf::Text> mText;
	int mTextAmount;

	//Sprite Variables
	std::vector<sf::Sprite> mSprites;
	int mSpriteAmount;

	//Mouse Variables
	sf::Vector2f mMousePos;

	//Player Variables
	sf::String mName;		//Player name, entered in enter name state
	int mPlayer = 0;		//Player type, player 1 has red pieces, player 2 has yellow pieces
};
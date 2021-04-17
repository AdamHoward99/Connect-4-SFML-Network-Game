#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameBoard.h"
#include "NetworkConnection.h"
#include "GameData.h"
#include "States.h"
#include "Menu.h"
#include <chrono>

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

	//Chat Functions
	bool GetIfChatIsOpen() { return isChatOpen; }

	//Turn Timer Variable
	std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point> mTurnTimer;

	//Name Functions
	void SetName(sf::String s) { mName = s; }

	//Set which player in the game via server functions
	int GetPlayer() { return player; }
	void SetPlayer(int p);

	bool IsPlayersTurn();		//Returns if the player can go

	GameData& GetData() { return mServer.mGameData; }

private:
	sf::RenderWindow& window;
	NetworkConnection& mServer;

	void AddPiece();
	void UpdateMousePosition();

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
	bool turnEnd = false;

	//Board & Piece Variables
	GameBoard board;
	sf::CircleShape pieceToAdd;
	sf::Vector2f mousePos;
	float xColumnPosition = 0.f;
	sf::Vector2i lastMove;		//Stores position of last move on board to easier check for a win

	//Sound Variables
	std::pair<sf::SoundBuffer, sf::Sound> mPieceSfx;

	//Font Variables
	sf::Font mFont;

	//Timer Variables
	void UpdateTurnTimer();
	void AutomaticPiecePlacement();

	//Chat Button Variables
	sf::Texture mButtonTex;

	//Chat Input Variables
	sf::String mChatInput;
	const unsigned int ChatLogCharacterLimit = 20;

	//Chat Log Panel Variables
	sf::Texture mChatPanelTex;
	bool isChatOpen = false;

	//Player Name
	sf::String mName;

	int player = 0;
	int mPieceColumn = 0;

	//Text Variables
	std::vector<sf::Text> mText;
	int TextAmount;

	//Sprite Variables
	std::vector<sf::Sprite> mSprites;
	int SpriteAmount;

};
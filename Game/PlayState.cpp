#include "PlayState.h"

PlayState::PlayState(sf::RenderWindow& mApp, NetworkConnection& connect)
	:mWindow(mApp), mGameBoard(mApp), mServer(connect)
{}

PlayState::~PlayState()
{
	//Destructor
	mGameBoard.~GameBoard();
}

void PlayState::Initialize()
{
	//Setup board and pieces
	mGameBoard.Initialize();
	mPieceToAdd = sf::CircleShape(30.f);

	//Setup GameData to be sent to server
	mServer.mGameData.mTurn = Turn::Player_1_Turn;
	mServer.mGameData.mLastMove = { -1, -1 };

	//Set amount of buttons and text on menu
	mTextAmount = 3;
	mText = std::vector<sf::Text>(mTextAmount);

	mSpriteAmount = 2;
	mSprites = std::vector<sf::Sprite>(mSpriteAmount);

	//Setup Functions
	SetupTextures();
	SetupSprites();
	SetupFonts();
	SetupText();
	SetupAudio();
}

void PlayState::SetupTextures()
{
	//Button Texture
	mButtonTex.loadFromFile("../bin/Textures/MenuButton.png");
	mButtonTex.setSmooth(true);

	//Chat Panel Texture
	mChatPanelTex.loadFromFile("../bin/Textures/ChatPanel.png");
	mChatPanelTex.setSmooth(true);
}

void PlayState::SetupSprites()
{
	//Chat Button Sprite
	mSprites.at(0).setTexture(mButtonTex);
	mSprites.at(0).setPosition(750.f, 670.f);
	mSprites.at(0).setScale(0.4f, 0.75f);

	//Chat Panel Sprite
	mSprites.at(1).setTexture(mChatPanelTex);
	mSprites.at(1).setPosition(900.f, 50.f);
	mSprites.at(1).setScale(1.f, 1.f);
}

void PlayState::SetupFonts()
{
	//Font
	if (!mFont.loadFromFile("../bin/Fonts/Komika_display.ttf"))
		exit(0);
}

void PlayState::SetupText()
{
	for (size_t i = 0; i < mText.size(); i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setFont(mFont);
	}

	//Pause Text
	mText.at(0).setCharacterSize(35);
	mText.at(0).setPosition(40.f, 740.f);

	//Chat Button Text
	mText.at(1).setString("Chat");
	mText.at(1).setCharacterSize(25);
	mText.at(1).setPosition(780.f, 680.f);

	//Chat Input Text
	mText.at(2).setFillColor(sf::Color::Black);
	mText.at(2).setPosition(620.f, 600.f);
	mText.at(2).setCharacterSize(15);
}

void PlayState::SetupAudio()
{
	//Audio
	mPieceSfx.first.loadFromFile("../bin/Music/PieceSfx.wav");
	mPieceSfx.second.setBuffer(mPieceSfx.first);
	mPieceSfx.second.setVolume(40.f);
}

void PlayState::Update()
{
	if (!mServer.GetDataUpdate())		//Obtains data from server, helps client detect when opponent has had their turn
	{
		mServer.mGameData.mDisconnected = -1;		//Send client back to main menu
		mServer.CloseConnection();
		return;
	}

	mGameBoard.Update(mServer.mGameData.mLastMove, mPlayer);		//Updates board if other client has placed a piece
	ChatUpdateServer();												//Updates chat if any information has been received from the server

	UpdateTurnTimer();			//Updates how long the player has left to move if its their turn
	UpdateMousePosition();

	//Switch turns when the current players turn is over
	if (mTurnEnd)
		TransitionTurn();
}

void PlayState::UpdateMousePosition()
{
	//Updating mouse position
	mMousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

	//Lock piece position to specific positions
	if (mMousePos.x <= 100.f)
	{
		mColumnPositionX = 50.f;
		mPieceColumn = 1;
	}
	else if (mMousePos.x <= 200.f)
	{
		mColumnPositionX = 150.f;
		mPieceColumn = 2;
	}
	else if (mMousePos.x <= 300.f)
	{
		mColumnPositionX = 250.f;
		mPieceColumn = 3;
	}
	else if (mMousePos.x <= 400.f)
	{
		mColumnPositionX = 350.f;
		mPieceColumn = 4;
	}
	else if (mMousePos.x <= 500.f)
	{
		mColumnPositionX = 450.f;
		mPieceColumn = 5;
	}
	else if (mMousePos.x <= 600.f)
	{
		mColumnPositionX = 550.f;
		mPieceColumn = 6;
	}
	else
	{
		mColumnPositionX = 650.f;
		mPieceColumn = 7;
	}

	mPieceToAdd.setPosition(mColumnPositionX, 20.f);
}

void PlayState::AddPiece()
{
	if (mGameBoard.PlacePiece(mPieceColumn, mPieceToAdd, mLastMove))		//Returns true when successfully added a piece into the board
	{
		mTurnEnd = true;
		mPieceSfx.second.play();		//Piece drop sfx
	}
}

void PlayState::Draw()
{
	mGameBoard.Draw();
	mWindow.draw(mSprites.at(0));	//Chat Button Sprite
	mWindow.draw(mText.at(1));		//Chat Button Text

	if (IsPlayersTurn())			//Only show piece and turn timer when its the players turn
	{
		mWindow.draw(mPieceToAdd);
		mWindow.draw(mText.at(0));
	}

	if (mChatOpen)					//Displays chat log when chat window is open
	{
		mWindow.draw(mSprites.at(1));
		mWindow.draw(mText.at(2));

		for (auto t : mChatLogText)
			mWindow.draw(t);
	}

}

void PlayState::Reset()
{
	//Reset board
	mGameBoard.ResetBoard();

	//Reset bools
	mChatOpen = false;

	//Reset Server Data
	mServer.mGameData.mDisconnected = 1;
	mServer.mGameData.mLastMove = std::pair<int, int>{ -1, -1 };
	mServer.mGameData.mMessage = "";
	mServer.mGameData.mTurn = Turn::Player_1_Turn;
	mServer.mGameData.gameEnded = false;

	//Reset Mouse position
	mMousePos = { -1.f, -1.f };		//Prevent piece being added immediately after rematching

	//Reset chat
	mChatInput.clear();
	mText.at(2).setString(mChatInput);
	mChatLog.clear();
	mChatLogText.clear();
}

void PlayState::UpdateTurnTimer()
{
	if (IsPlayersTurn())		//Only count down turn timer when its the players turn
	{
		mTurnTimer.second = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(mTurnTimer.second - mTurnTimer.first).count() / 1000000.f;		//Gets difference in time

		mText.at(0).setString(std::to_string(static_cast<int>(30 - elapsedTime)));		//Shows player how long they have left on the pause menu

		if (elapsedTime > 30.f)
		{
			AutomaticPiecePlacement();	//Function to place a piece automatically
		}
	}
	else
		mTurnTimer.first = std::chrono::steady_clock::now();		//Keep refreshing the start time until its the players turn again
}

void PlayState::AutomaticPiecePlacement()
{
	if (mGameBoard.AutomaticPiecePlace(mPieceToAdd, mLastMove))		//Piece was able to be added to the board
	{
		mTurnEnd = true;
		mPieceSfx.second.play();		//Piece drop sfx
	}
}

void PlayState::ButtonPress()
{
	if (mSprites.at(0).getGlobalBounds().contains(mMousePos))		//Click within chat button dimensions
	{
		if (!mChatOpen)
		{
			mSprites.at(1).setPosition(600.f, 50.f);		//Make chat panel visible
			mChatOpen = true;
		}
		else
		{
			mSprites.at(1).setPosition(900.f, 50.f);		//Make chat panel not visible
			mChatOpen = false;
		}
	}
	else
		if (!mSprites.at(1).getGlobalBounds().contains(mMousePos) && IsPlayersTurn() && mMousePos.x > 0)	//Add piece to board
			AddPiece();
}

void PlayState::ChatInput(sf::Event ev)
{
	if (ev.type == sf::Event::TextEntered && mChatOpen)		//Only allow chat input when chat window is open
	{
		if (ev.text.unicode == ENTER_KEY)		//Add entered string into chat log
		{
			if (mChatInput.getSize() > 0)		//Make sure entered string has at least 1 character before adding
				UpdateChatLog();
		}

		else if (ev.text.unicode == BACKSPACE_KEY)		//Remove char from entered string
		{
			if (mChatInput.getSize() > 0)				//Prevent string going out of range into negatives
			{
				mChatInput.erase(mChatInput.getSize() - 1);
				mText.at(2).setString(mChatInput);		//Display changes to string
			}

		}

		else if (ev.text.unicode == SPACE_KEY && mChatInput.getSize() < 1)		//Prevents empty messages from being added to chat log
			return;

		else				//Add entered key into the string
		{
			if (mChatInput.getSize() < mChatLogCharacterLimit)		//Prevents typing over 20 characters
			{
				mChatInput += ev.text.unicode;
				mText.at(2).setString(mChatInput);
			}
		}
	}
}

void PlayState::UpdateChatLog()
{
	//Prevents chat log going above chat panel sprite
	if (mChatLog.size() <= 23)
	{
		mChatLog.push_back(mName + ": " + mChatInput);		//Add entered line into the string
		sf::Text t;
		mChatLogText.push_back(t);		//Add to log to be displayed on screen
	}
	else
	{
		mChatLog.erase(mChatLog.begin() + 0);		//Removes first chat log when it goes out of bounds of chat panel sprite
		mChatLogText.erase(mChatLogText.begin() + 0);

		mChatLog.push_back(mName + ": " + mChatInput);		//Add entered line into the string
		sf::Text t;
		mChatLogText.push_back(t);
	}

	mServer.mGameData.mMessage = mName + ": " + mChatInput;		//Collect message sent to relay to other client via server

	//Send Chat message to server
	if (!mServer.SendGameData(mServer.mGameData))
	{
		mServer.CloseConnection();
		return;
	}

	mServer.mGameData.mMessage = "";		//Reset chat message to prevent message being sent again by the server

	mChatInput.clear();						//Clear entered string
	mText.at(2).setString(mChatInput);		//Display empty string on screen

	//Outputs all chat log messages
	float yOffset = 520.f;
	for (int i = mChatLog.size() - 1; i >= 0; i--)
	{
		mChatLogText.at(i).setString(mChatLog.at(i));
		mChatLogText.at(i).setFillColor(sf::Color::Black);
		mChatLogText.at(i).setFont(mFont);
		mChatLogText.at(i).setCharacterSize(15);
		mChatLogText.at(i).setPosition(620.f, yOffset);
		yOffset -= 20.f;
	}
}

void PlayState::SetPlayer(int p)
{
	mPlayer = p;

	switch (mPlayer)
	{
	case 1:
		mPieceToAdd.setFillColor(sf::Color::Red);
		break;

	case 2:
		mPieceToAdd.setFillColor(sf::Color::Yellow);
		break;

	default:
		mServer.CloseConnection();		//An incorrect value was obtained
		break;
	}

}

void PlayState::ChatUpdateServer()
{
	//Update chat log if any message was received from the server
	if (mServer.mGameData.mMessage != "")		//String received from server contains characters
	{
		if (mChatLog.size() <= 23)				//Add string from server to chat log
		{
			mChatLog.push_back(mServer.mGameData.mMessage);		//Add entered line into the string
			sf::Text t;
			mChatLogText.push_back(t);
		}
		else
		{
			mChatLog.erase(mChatLog.begin() + 0);		//Removes first chat log
			mChatLogText.erase(mChatLogText.begin() + 0);

			mChatLog.push_back(mServer.mGameData.mMessage);		//Add entered line into the string
			sf::Text t;
			mChatLogText.push_back(t);
		}

		mServer.mGameData.mMessage = "";		//Reset message back to default empty

		//Outputs all chat log messages
		float yOffset = 520.f;
		for (int i = mChatLog.size() - 1; i >= 0; i--)
		{
			mChatLogText.at(i).setString(mChatLog.at(i));
			mChatLogText.at(i).setFillColor(sf::Color::Black);
			mChatLogText.at(i).setFont(mFont);
			mChatLogText.at(i).setCharacterSize(15);
			mChatLogText.at(i).setPosition(620.f, yOffset);
			yOffset -= 20.f;
		}
	}
}

void PlayState::TransitionTurn()
{
	//Function to check if 4 are connected
	if (mGameBoard.Connected4(mPlayer, mLastMove))
	{
		//Pass information that this player has won to the server
		mServer.mGameData.gameEnded = true;

		//Set win message based on which player this is
		mServer.mGameData.mWinMessage = mName + " Wins";
	}

	if (mGameBoard.CheckIfBoardIsFull())		//Checks if there are no more available slots in the board, results in a tie
	{
		//Pass information that no player has won to the server
		mServer.mGameData.gameEnded = true;
		mServer.mGameData.mWinMessage = "Its a Tie";
	}

	SwapPlayerTurn();

	mServer.mGameData.mLastMove = { mLastMove.x, mLastMove.y };		//Get last move of this player to send to server to update the board for the other client

	//Send turn changes and board update to the server
	if (!mServer.SendGameData(mServer.mGameData))
	{
		mServer.CloseConnection();
		return;
	}

	//Reset Values
	mServer.mGameData.mLastMove = { -1, -1 };
	mTurnEnd = false;
}

void PlayState::SwapPlayerTurn()
{
	//Swap turns based on which player type this player is
	if (mServer.mGameData.mTurn == Turn::Player_1_Turn)
		mServer.mGameData.mTurn = Turn::Player_2_Turn;
	else
		mServer.mGameData.mTurn = Turn::Player_1_Turn;
}
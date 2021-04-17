#include "PlayState.h"

PlayState::PlayState(sf::RenderWindow& mApp, NetworkConnection& connect)
	:window(mApp), board(mApp), mServer(connect)
{

}

PlayState::~PlayState()
{
	//Destructor
	board.~GameBoard();
}

void PlayState::Initialize()
{
	board.Initialize();
	pieceToAdd = sf::CircleShape(30.f);
	mServer.mGameData.mTurn = Turn::Player_1_Turn;
	mServer.mGameData.mLastMove = { -1, -1 };

	TextAmount = 3;
	mText = std::vector<sf::Text>(TextAmount);

	SpriteAmount = 2;
	mSprites = std::vector<sf::Sprite>(SpriteAmount);

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
	if (!mButtonTex.loadFromFile("bin/Textures/MenuButton.png"))
		assert(!mButtonTex.loadFromFile("bin/Textures/MenuButton.png"));

	mButtonTex.setSmooth(true);

	//Chat Texture
	if (!mChatPanelTex.loadFromFile("bin/Textures/ChatPanel.png"))
		assert(!mChatPanelTex.loadFromFile("bin/Textures/ChatPanel.png"));

	mChatPanelTex.setSmooth(true);

}

void PlayState::SetupSprites()
{
	//Chat Button
	mSprites.at(0).setTexture(mButtonTex);
	mSprites.at(0).setPosition(750.f, 670.f);
	mSprites.at(0).setScale(0.4f, 0.75f);

	//Chat Panel
	mSprites.at(1).setTexture(mChatPanelTex);
	mSprites.at(1).setPosition(900.f, 50.f);
	mSprites.at(1).setScale(1.f, 1.f);
}

void PlayState::SetupFonts()
{
	//Font
	if (!mFont.loadFromFile("bin/Fonts/Komika_display.ttf"))
		assert(!mFont.loadFromFile("bin/Fonts/Komika_display.ttf"));
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
	if (!mPieceSfx.first.loadFromFile("bin/Music/PieceSfx.wav"))
		assert(!mPieceSfx.first.loadFromFile("bin/Music/PieceSfx.wav"));

	mPieceSfx.second.setBuffer(mPieceSfx.first);
	mPieceSfx.second.setVolume(40.f);
}

void PlayState::Update()
{
	//Update variables of the game, turn, board update, chat
	if (!mServer.GetDataUpdate())
	{
		mServer.CloseConnection();
		return;
	}

	if (mServer.mGameData.mDisconnected == -1)		//Disconnection on other clients end
	{
		mServer.CloseConnection();
		return;
	}

	board.Update(mServer.mGameData.mLastMove, player);		//Updates board if other client has placed a piece
	ChatUpdateServer();		//Updates chat if any information has been received from the server

	if (IsPlayersTurn())
	{
		UpdateTurnTimer();
	}
	else
		mTurnTimer.first = std::chrono::steady_clock::now();

	UpdateMousePosition();

	//Switch turns, put into its own function
	if (turnEnd)
	{
		//Function to check if 4 are connected
		if (board.Connected4(player, lastMove))
		{
			//Pass information that this player has won to the server, returns win screen string
			mServer.mGameData.gameEnded = true;

			//Set win message based on which player this is
			mServer.mGameData.mWinMessage = mName + " Wins";
		}

		//Function to see if there is a winner
		if (board.CheckIfBoardIsFull())
		{
			//Pass information that no player has won to the server,returns win screen string
			mServer.mGameData.gameEnded = true;
			mServer.mGameData.mWinMessage = "Its a Tie";
		}

		//Swaps turns, passes this to server to relay to other clients, put all this in its own function?
		if (mServer.mGameData.mTurn == Turn::Player_1_Turn)
			mServer.mGameData.mTurn = Turn::Player_2_Turn;
		else
			mServer.mGameData.mTurn = Turn::Player_1_Turn;

		mServer.mGameData.mLastMove = {lastMove.x, lastMove.y};

		//Send turn changes to server
		if (!mServer.SendGameData(mServer.mGameData))
		{
			mServer.CloseConnection();
			return;
		}

		mServer.mGameData.mLastMove = { -1, -1 };
		turnEnd = false;
	}

}

void PlayState::UpdateMousePosition()
{
	//Updating mouse position
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	//Lock piece position to specific positions
	if (mousePos.x <= 100.f)
	{
		xColumnPosition = 50.f;
		mPieceColumn = 1;
	}
	else if (mousePos.x <= 200.f)
	{
		xColumnPosition = 150.f;
		mPieceColumn = 2;
	}
	else if (mousePos.x <= 300.f)
	{
		xColumnPosition = 250.f;
		mPieceColumn = 3;
	}
	else if (mousePos.x <= 400.f)
	{
		xColumnPosition = 350.f;
		mPieceColumn = 4;
	}
	else if (mousePos.x <= 500.f)
	{
		xColumnPosition = 450.f;
		mPieceColumn = 5;
	}
	else if (mousePos.x <= 600.f)
	{
		xColumnPosition = 550.f;
		mPieceColumn = 6;
	}
	else
	{
		xColumnPosition = 650.f;
		mPieceColumn = 7;
	}

	pieceToAdd.setPosition(xColumnPosition, 20.f);
}

void PlayState::AddPiece()
{
	if (board.PlacePiece(mPieceColumn, pieceToAdd, lastMove))		//Returns true when successfully added a piece into the board
	{
		turnEnd = true;
		mPieceSfx.second.play();		//Piece drop sfx
	}
}

void PlayState::Draw()
{
	board.Draw();
	window.draw(mSprites.at(0));
	window.draw(mText.at(1));

	if (IsPlayersTurn())
	{
		window.draw(pieceToAdd);
		window.draw(mText.at(0));
	}

	if (isChatOpen)
	{
		window.draw(mSprites.at(1));
		window.draw(mText.at(2));
		for (auto t : mChatLogText)
			window.draw(t);
	}

}

void PlayState::Reset()
{
	//Reset board
	board.ResetBoard();

	//Reset bools
	isChatOpen = false;

	//Reset Server Data
	mServer.mGameData.mDisconnected = 1;
	mServer.mGameData.mLastMove = std::pair<int, int>{ -1, -1 };
	mServer.mGameData.mMessage = "";
	mServer.mGameData.mTurn = Turn::Player_1_Turn;
	mServer.mGameData.gameEnded = false;

	//Reset chat
	mChatInput.clear();
	mText.at(2).setString(mChatInput);
	mChatLog.clear();
	mChatLogText.clear();
}

void PlayState::UpdateTurnTimer()
{
	mTurnTimer.second = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(mTurnTimer.second - mTurnTimer.first).count() / 1000000.f;

	mText.at(0).setString(std::to_string(static_cast<int>(30 - elapsedTime)));		//Shows player how long they have left on the pause menu

	if (elapsedTime > 30.f)
	{
		AutomaticPiecePlacement();	//Function to place a piece automatically
	}
}

void PlayState::AutomaticPiecePlacement()
{
	if (board.AutomaticPiecePlace(pieceToAdd, lastMove))		//Piece was able to be added to the board
	{
		turnEnd = true;
		mPieceSfx.second.play();		//Piece drop sfx
	}
}

void PlayState::ButtonPress()
{
	if (mSprites.at(0).getGlobalBounds().contains(mousePos))
	{
		if (!isChatOpen) 
		{
			mSprites.at(1).setPosition(600.f, 50.f);		//Make chat panel visible
			isChatOpen = true;
		}
		else
		{
			mSprites.at(1).setPosition(900.f, 50.f);		//Make chat panel not visible
			isChatOpen = false;
		}
	}
	else
		if(!mSprites.at(1).getGlobalBounds().contains(mousePos) && IsPlayersTurn() && !turnEnd)
			AddPiece();
}

void PlayState::ChatInput(sf::Event ev)
{
	if (ev.type == sf::Event::TextEntered && isChatOpen)
	{
		if (ev.text.unicode == ENTER_KEY)
		{
			if (mChatInput.getSize() > 0)
				UpdateChatLog();
		}

		else if (ev.text.unicode == BACKSPACE_KEY)
		{
			if (mChatInput.getSize() > 0)
			{
				mChatInput.erase(mChatInput.getSize() - 1);
				mText.at(2).setString(mChatInput);
			}

		}

		else if (ev.text.unicode == SPACE_KEY && mChatInput.getSize() < 1)		//Prevents empty messages from being added to chat log
			return;

		else
		{
			if (mChatInput.getSize() < ChatLogCharacterLimit)		//Prevents typing over 20 characters
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
		mChatLogText.push_back(t);
	}
	else
	{
		mChatLog.erase(mChatLog.begin() + 0);		//Removes first chat log
		mChatLogText.erase(mChatLogText.begin() + 0);

		mChatLog.push_back(mName + ": " + mChatInput);		//Add entered line into the string
		sf::Text t;
		mChatLogText.push_back(t);
	}

	mServer.mGameData.mMessage = mName + ": " + mChatInput;

	//Send Chat message to server
	if (!mServer.SendGameData(mServer.mGameData))
	{
		mServer.CloseConnection();
		return;
	}

	mServer.mGameData.mMessage = "";

	mChatInput.clear();
	mText.at(2).setString(mChatInput);

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
	player = p;

	switch (player)
	{
	case 1:
		pieceToAdd.setFillColor(sf::Color::Red);
		OutputDebugStringA("\nSets as player 1...");
		break;

	case 2:
		pieceToAdd.setFillColor(sf::Color::Yellow);
		OutputDebugStringA("\nSets as player 2...");
		break;

	default:
		OutputDebugStringA("\nGets default, no value, closes the connection...");
		mServer.CloseConnection();
		break;
	}

}

bool PlayState::IsPlayersTurn()
{
	return mServer.mGameData.mTurn == player;
}

void PlayState::ChatUpdateServer()
{
	//Update chat log if any message was received from the server
	if (mServer.mGameData.mMessage != "")
	{
		if (mChatLog.size() <= 23)
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

		mServer.mGameData.mMessage = "";

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

bool PlayState::PlayerDisconnected()
{
	return GetData().mDisconnected == -1;
}

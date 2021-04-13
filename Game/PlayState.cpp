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

	BoardUpdateServer();	//Updates board if other client placed a piece
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
		if (HasConnected4())
		{
			//Pass information that this player has won to the server, returns win screen string
			mServer.mGameData.gameEnded = true;

			//Set win message based on which player this is
			if (player == 1)
				mServer.mGameData.mWinMessage = "Player 1 Wins";
			else if (player == 2)
				mServer.mGameData.mWinMessage = "Player 2 Wins";
		}

		//Function to see if there is a winner
		if (IsBoardFull())
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

	//Todo: could make this look better
	if (mousePos.x <= 100.f)
		xColumnPosition = 50.f;
	else if (mousePos.x <= 200.f)
		xColumnPosition = 150.f;
	else if (mousePos.x <= 300.f)
		xColumnPosition = 250.f;
	else if (mousePos.x <= 400.f)
		xColumnPosition = 350.f;
	else if (mousePos.x <= 500.f)
		xColumnPosition = 450.f;
	else if (mousePos.x <= 600.f)
		xColumnPosition = 550.f;
	else if (mousePos.x <= 700.f)
		xColumnPosition = 650.f;
	else
		xColumnPosition = 650.f;

	pieceToAdd.setPosition(xColumnPosition, 20.f);
}

bool PlayState::IsBoardFull()
{
	//See if all slots are filled, tie in this case
	bool boardIsFull;
	for (int y = 1; y < BOARD_WIDTH; y++)
	{
		if (board.pieces[1][y].getFillColor() == sf::Color::White)		//Checks if all top slots are filled
		{
			boardIsFull = false;
			break;
		}
		else
			boardIsFull = true;
	}

	return boardIsFull;
}

bool PlayState::HasConnected4()
{
	bool foundaWinner = false;
	int connected = 0;		//Amount of pieces of the same colour connected together

	sf::Color c;

	if (mServer.mGameData.mTurn == Turn::Player_1_Turn)
		c = sf::Color::Red;
	else
		c = sf::Color::Yellow;
	
	//Could separate into different functions
	//Horizontal checks
	int y = lastMove.y;
	while (y >= 1 && board.pieces[lastMove.x][y].getFillColor() == c)		//Horizontal Left searching
	{
		y--;
		connected++;
		if (connected >= 4)
			return true;
	}

	connected--;		//Remove as the next will readd the most recent piece added
	y = lastMove.y;
	while (y < BOARD_WIDTH && board.pieces[lastMove.x][y].getFillColor() == c)	//Horizontal Right searching
	{
		y++;
		connected++;
		if (connected >= 4)
			return true;
	}

	//Vertical checks
	connected = 0;
	int x = lastMove.x;
	while (x >= 1 && board.pieces[x][lastMove.y].getFillColor() == c)			//Vertical searching
	{
		x++;
		connected++;
		if (connected >= 4)
			return true;
	}

	//Diagonal checks
	connected = 0;
	x = lastMove.x;
	y = lastMove.y;
	while (x >= 1 && y < BOARD_WIDTH && board.pieces[x][y].getFillColor() == c)		//North-east movement
	{
		x--;
		y++;
		connected++;
		if (connected >= 4)
			return true;
	}

	connected--;
	x = lastMove.x;
	y = lastMove.y;
	while (x < BOARD_HEIGHT && y >= 1 && board.pieces[x][y].getFillColor() == c)	//South-west movement
	{
		x++;
		y--;
		connected++;
		if (connected >= 4)
			return true;
	}

	connected = 0;
	x = lastMove.x;
	y = lastMove.y;
	while (x < BOARD_HEIGHT && y < BOARD_WIDTH && board.pieces[x][y].getFillColor() == c)		//South-east movement
	{
		x++;
		y++;
		connected++;
		if (connected >= 4)
			return true;
	}

	connected--;
	x = lastMove.x;
	y = lastMove.y;
	while (x >= 1 && y >= 1 && board.pieces[x][y].getFillColor() == c)		//North-west searching
	{
		x--;
		y--;
		connected++;
		if (connected >= 4)
			return true;
	}

	return false;
}

void PlayState::PlacePiece()
{
	int col;
	if (xColumnPosition == 50.f)
		col = 1;
	else if (xColumnPosition == 150.f)
		col = 2;
	else if (xColumnPosition == 250.f)
		col = 3;
	else if (xColumnPosition == 350.f)
		col = 4;
	else if (xColumnPosition == 450.f)
		col = 5;
	else if (xColumnPosition == 550.f)
		col = 6;
	else
		col = 7;

	for (int i = BOARD_HEIGHT - 1; i >= 1; i--)
	{
		if (board.pieces[i][col].getFillColor() == sf::Color::White)		//Finds an empty piece
		{
			//Add stuff for when the column is full
			pieceToAdd.setPosition(board.pieces[i][col].getPosition());
			board.pieces[i][col] = pieceToAdd;
			turnEnd = true;
			lastMove = sf::Vector2i(i, col);
			mPieceSfx.second.play();
			break;
		}
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
	for (int y = 1; y < BOARD_WIDTH; y++)
	{
		for (int i = BOARD_HEIGHT - 1; i >= 1; i--)
		{
			if (board.pieces[i][y].getFillColor() == sf::Color::White)		//Finds an empty piece
			{
				//Add stuff for when the column is full
				pieceToAdd.setPosition(board.pieces[i][y].getPosition());
				board.pieces[i][y] = pieceToAdd;
				turnEnd = true;
				lastMove = sf::Vector2i(i, y);
				mPieceSfx.second.play();
				y = BOARD_WIDTH;
				break;
			}
		}
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
		if(!mSprites.at(1).getGlobalBounds().contains(mousePos) && IsPlayersTurn())
			PlacePiece();
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
			if (mChatInput.getSize() < ChatLogCharacterLimit)		//Prevents typing over 25 characters
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
		mChatLog.push_back(mName + ": " + mChatInput += "\n");		//Add entered line into the string
		sf::Text t;
		mChatLogText.push_back(t);
	}
	else
	{
		mChatLog.erase(mChatLog.begin() + 0);		//Removes first chat log
		mChatLogText.erase(mChatLogText.begin() + 0);

		mChatLog.push_back(mChatInput += "\n");		//Add entered line into the string
		sf::Text t;
		mChatLogText.push_back(t);
	}

	mServer.mGameData.mMessage = mName + ": " + mChatInput += "\n";

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

void PlayState::BoardUpdateServer()
{
	//Update appearance of the game board to reflect other players turn
	if (mServer.mGameData.mLastMove != std::pair<int, int>{-1, -1})
	{
		sf::CircleShape piece = sf::CircleShape(30.f);
		piece.setPosition(board.pieces[mServer.mGameData.mLastMove.first][mServer.mGameData.mLastMove.second].getPosition());

		if (player == 1)
			piece.setFillColor(sf::Color::Yellow);
		else
			piece.setFillColor(sf::Color::Red);

		board.pieces[mServer.mGameData.mLastMove.first][mServer.mGameData.mLastMove.second] = piece;
		mServer.mGameData.mLastMove = { -1, -1 };
	}
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

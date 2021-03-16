#include "PlayState.h"

PlayState::PlayState(sf::RenderWindow& mApp)
	:window(mApp), board(mApp)
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
	DecideTurnOrder();

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
}

void PlayState::SetupSprites()
{
	//Chat Button
	mChatButton.setTexture(mButtonTex);
	mChatButton.setPosition(750.f, 670.f);
	mChatButton.setScale(0.4f, 0.75f);
}

void PlayState::SetupFonts()
{
	//Font
	if (!mFont.loadFromFile("bin/Fonts/Komika_display.ttf"))
		assert(!mFont.loadFromFile("bin/Fonts/Komika_display.ttf"));
}

void PlayState::SetupText()
{
	//Pause Text
	mTimerText.setFillColor(sf::Color::White);
	mTimerText.setCharacterSize(35);
	mTimerText.setFont(mFont);
	mTimerText.setPosition(40.f, 740.f);

	//Chat Text
	mChatButtonText.setString("Chat");
	mChatButtonText.setFillColor(sf::Color::White);
	mChatButtonText.setCharacterSize(25);
	mChatButtonText.setFont(mFont);
	mChatButtonText.setPosition(780.f, 680.f);
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
	board.Update();

	UpdateTurnTimer();
	UpdateMousePosition();

	//Switch turns, put into its own function
	if (turnEnd)
	{
		//Function to check if 4 are connected
		if (HasConnected4())
		{
			gameWon = true;

			if (mGameTurn == Turn::Player_1_Turn)
				winMessage = "Player 1 Wins";
			else
				winMessage = "Player 2 Wins";
		}

		//Function to see if there is a winner
		if (IsBoardFull())
		{
			gameWon = true;
			winMessage = " It's a Tie";
		}

		SwitchTurns();		//Changes piece colour and turn variable, sets turnEnd to false
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
	for (int y = 0; y < BOARD_WIDTH; y++)
	{
		if (board.pieces[0][y].getFillColor() == sf::Color::White)		//Checks if all top slots are filled
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

	if (mGameTurn == Turn::Player_1_Turn)
		c = sf::Color::Red;
	else
		c = sf::Color::Yellow;
	
	//Could separate into different functions
	//Horizontal checks
	int y = lastMove.y;
	while (y >= 0 && board.pieces[lastMove.x][y].getFillColor() == c)		//Horizontal Left searching
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
	while (x >= 0 && board.pieces[x][lastMove.y].getFillColor() == c)			//Vertical searching
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
	while (x >= 0 && y < BOARD_WIDTH && board.pieces[x][y].getFillColor() == c)		//North-east movement
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
	while (x < BOARD_HEIGHT && y >= 0 && board.pieces[x][y].getFillColor() == c)	//South-west movement
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
	while (x >= 0 && y >= 0 && board.pieces[x][y].getFillColor() == c)		//North-west searching
	{
		x--;
		y--;
		connected++;
		if (connected >= 4)
			return true;
	}

	return false;
}

void PlayState::DecideTurnOrder()
{
	int random = std::rand() % 100 + 1;

	if (random < 49)
	{
		mGameTurn = Turn::Player_1_Turn;
		pieceToAdd.setFillColor(sf::Color::Red);
	}
	else
	{
		mGameTurn = Turn::Player_2_Turn;
		pieceToAdd.setFillColor(sf::Color::Yellow);
	}
}

void PlayState::SwitchTurns()
{
	if (mGameTurn == Turn::Player_1_Turn)
	{
		mGameTurn = Turn::Player_2_Turn;
		pieceToAdd.setFillColor(sf::Color::Yellow);
	}
	else
	{
		mGameTurn = Turn::Player_1_Turn;
		pieceToAdd.setFillColor(sf::Color::Red);
	}

	mTurnTimer.first = std::chrono::steady_clock::now();

	turnEnd = false;
}

void PlayState::PlacePiece()
{
	int col;
	if (xColumnPosition == 50.f)
		col = 0;
	else if (xColumnPosition == 150.f)
		col = 1;
	else if (xColumnPosition == 250.f)
		col = 2;
	else if (xColumnPosition == 350.f)
		col = 3;
	else if (xColumnPosition == 450.f)
		col = 4;
	else if (xColumnPosition == 550.f)
		col = 5;
	else
		col = 6;

	for (int i = BOARD_HEIGHT - 1; i >= 0; i--)
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
	window.draw(pieceToAdd);
	window.draw(mTimerText);
	window.draw(mChatButton);
	window.draw(mChatButtonText);
}

void PlayState::Reset()
{
	board.ResetBoard();
	gameWon = false;
	DecideTurnOrder();
}

void PlayState::UpdateTurnTimer()
{
	mTurnTimer.second = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(mTurnTimer.second - mTurnTimer.first).count() / 1000000.f;

	mTimerText.setString(std::to_string(static_cast<int>(30 - elapsedTime)));		//Shows player how long they have left on the pause menu

	if (elapsedTime > 30.f)
	{
		AutomaticPiecePlacement();	//Function to place a piece automatically
	}
}

void PlayState::AutomaticPiecePlacement()
{
	for (int y = 0; y < BOARD_WIDTH; y++)
	{
		for (int i = BOARD_HEIGHT - 1; i >= 0; i--)
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
	if (mChatButton.getGlobalBounds().contains(mousePos))
	{
		float f = 1.f;
	}
	else
		PlacePiece();
}
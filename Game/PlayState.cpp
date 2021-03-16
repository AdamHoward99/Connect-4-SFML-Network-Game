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

	//Audio
	if (!mPieceSfx.first.loadFromFile("bin/Music/PieceSfx.wav"))
		assert(!mPieceSfx.first.loadFromFile("bin/Music/PieceSfx.wav"));

	mPieceSfx.second.setBuffer(mPieceSfx.first);
	mPieceSfx.second.setVolume(40.f);
}

void PlayState::Update()
{
	board.Update();

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
	mousePos = sf::Mouse::getPosition(window);

	//Todo: could make this look better
	if (mousePos.x <= 100)
		mousePos.x = 50;
	else if (mousePos.x <= 200)
		mousePos.x = 150;
	else if (mousePos.x <= 300)
		mousePos.x = 250;
	else if (mousePos.x <= 400)
		mousePos.x = 350;
	else if (mousePos.x <= 500)
		mousePos.x = 450;
	else if (mousePos.x <= 600)
		mousePos.x = 550;
	else if (mousePos.x <= 700)
		mousePos.x = 650;
	else
		mousePos.x = 650;

	pieceToAdd.setPosition(static_cast<float>(mousePos.x), 20.f);
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

	turnEnd = false;
}

void PlayState::PlacePiece()
{
	int col;
	if (mousePos.x == 50.f)
		col = 0;
	else if (mousePos.x == 150.f)
		col = 1;
	else if (mousePos.x == 250.f)
		col = 2;
	else if (mousePos.x == 350.f)
		col = 3;
	else if (mousePos.x == 450.f)
		col = 4;
	else if (mousePos.x == 550.f)
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
}

void PlayState::Reset()
{
	board.ResetBoard();
	gameWon = false;
	DecideTurnOrder();
}
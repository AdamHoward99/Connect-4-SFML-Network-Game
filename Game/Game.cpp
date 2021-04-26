#include "Game.h"

Game::Game(sf::RenderWindow& w, NetworkConnection& mConnect)
	:mWindow(w), mConnection(mConnect), mPlayState(w, mConnect)
{
	//Menus Initialization
	mMenus["StartMenu"] = std::make_unique<StartMenu>(w);
	mMenus["ControlMenu"] = std::make_unique<ControlMenu>(w);
	mMenus["EnterNameMenu"] = std::make_unique<EnterNameMenu>(w);
	mMenus["PauseMenu"] = std::make_unique<PauseMenu>(w);
	mMenus["WinMenu"] = std::make_unique<WinMenu>(w, mConnect);
	mMenus["DisconnectMenu"] = std::make_unique<DisconnectMenu>(w);

	//Setup elements of game
	Initialize();
}

Game::~Game()
{
	mConnection.CloseConnection();

	//Remove all pointers
	mMenus["StartMenu"].reset();
	mMenus["ControlMenu"].reset();
	mMenus["EnterNameMenu"].reset();
	mMenus["PauseMenu"].reset();
	mMenus["WinMenu"].reset();
	mMenus["DisconnectMenu"].reset();

}

void Game::Initialize()
{
	//Setup all elements of the game, not needed as constructors of each call initialize
	mPlayState.Initialize();

	if (!mTextFont.loadFromFile("../bin/Fonts/Komika_display.ttf"))
		exit(0);

	//Pause Text
	mPauseTimerTxt.setFillColor(sf::Color::White);
	mPauseTimerTxt.setCharacterSize(25);
	mPauseTimerTxt.setFont(mTextFont);
	mPauseTimerTxt.setPosition(590.f, 210.f);

	InitializeMatchmakingScreen();
}

void Game::InitializeMatchmakingScreen()
{
	//Background Texture
	mBackgroundTex.loadFromFile("../bin/Textures/menuBackground.png");
	mBackgroundTex.setSmooth(true);

	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Loading Text
	mLoadingText.setFillColor(sf::Color::White);
	mLoadingText.setFont(mTextFont);
	mLoadingText.setCharacterSize(25);
	mLoadingText.setPosition(320.f, 300.f);
	mLoadingText.setString("Finding an Opponent...");
}

void Game::Update()
{
	//Update each element of the game based on which state is active
	switch (mStates)
	{
	case States::Play:
		mPlayState.Update();

		if (mPlayState.PlayerDisconnected())		//If disconnection has happened
		{
			mPlayState.Reset();
			mStates = States::Disconnect_Menu;
		}

		if (mPlayState.GetData().gameEnded)			//A player has won the game or the game board is full
		{
			mPlayState.Reset();
			mStates = States::Win_Menu;
		}
		break;

	case States::Matchmaking:
		//Connect to the server
		if (!mConnection.ConnectToServer())
		{
			mStates = States::Start_Menu;
			break;
		}

		//Send the name of the player to the server
		if(!mConnection.SendPlayerName(mPlayerName))
		{
			mStates = States::Start_Menu;
			mConnection.CloseConnection();
			break;
		}
	
		//Send info to server to check if matchmaking is possible (> 1 available clients on server)
		if (!mConnection.Matchmake())		//Couldn't find an opponent
		{
			mConnection.CloseConnection();
			mStates = States::Start_Menu;
		}

		else				//Found an opponent
		{
			mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();
			mStates = States::Play;

			//Find from server which player this is (only returns either 1 or 2)
			int player = 0;

			if (!mConnection.GetPlayer(player))
			{
				mConnection.CloseConnection();
				mStates = States::Start_Menu;
				return;
			}
			mPlayState.SetPlayer(player);

			//Send Which player will be going first, is obtained from server in PlayState::Update
			if (!mConnection.SendGameData(mPlayState.GetData()))
			{
				mConnection.CloseConnection();
				mStates = States::Start_Menu;
			}

		}

		break;

	case States::Enter_Name:
		mMenus["EnterNameMenu"].get()->Update();
		break;

	case States::Pause_Menu:
		mMenus["PauseMenu"].get()->Update();
		UpdatePauseTimer();
		break;

	case States::Control_Menu:
		mMenus["ControlMenu"].get()->Update();
		break;

	case States::Start_Menu:
		mPlayState.SetPlayer(0);		//Resets player type back to null until it reconnects to the server
		mMenus["StartMenu"].get()->Update();
		break;

	case States::Win_Menu:
		mMenus["WinMenu"].get()->Update();
		break;

	case States::Disconnect_Menu:
		mMenus["DisconnectMenu"].get()->Update();
		break;

	case States::Quit:
		mWindow.close();
		break;

	default:
		exit(0);
		break;
	}

}

void Game::StartPauseTimer()
{
	mPauseTimer.first = std::chrono::steady_clock::now();
}

void Game::UpdatePauseTimer()
{
	mPauseTimer.second = std::chrono::steady_clock::now();		//Get the current time
	auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(mPauseTimer.second - mPauseTimer.first).count() / 1000000.f;		//See if the allowance time has passed

	mPauseTimerTxt.setString(std::to_string(static_cast<int>(mPauseTimerAllowance - elapsedTime)));		//Shows player how long they have left on the pause menu

	if (elapsedTime > mPauseTimerAllowance)		//More than 30 seconds have passed
	{
		mStates = States::Play;
		mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();
		if(mPauseTimerAllowance > 10)
			mPauseTimerAllowance *= 0.5f;		//Shortens next timer for pause menu to prevent endless pausing
	}
}

void Game::Draw()
{
	//Draw elements of the game based on which state is active
	switch (mStates)
	{
	case States::Play:
		mPlayState.Draw();
		break;

	case States::Matchmaking:
		DrawMatchmakingScreen();
		break;

	case States::Enter_Name:
		mMenus["EnterNameMenu"].get()->Draw();
		break;

	case States::Pause_Menu:
		mPlayState.Draw();
		mMenus["PauseMenu"].get()->Draw();
		mWindow.draw(mPauseTimerTxt);
		break;

	case States::Control_Menu:
		mMenus["ControlMenu"].get()->Draw();
		break;

	case States::Start_Menu:
		mMenus["StartMenu"].get()->Draw();
		break;

	case States::Win_Menu:
		mMenus["WinMenu"].get()->Draw();
		break;

	case States::Disconnect_Menu:
		mPlayState.Draw();
		mMenus["DisconnectMenu"].get()->Draw();
		break;

	default:
		exit(0);
		break;
	}
}

void Game::DrawMatchmakingScreen()
{
	mWindow.draw(mBackgroundSpr);
	mWindow.draw(mLoadingText);
}

void Game::MouseReleased(sf::Event ev)
{
	if (ev.mouseButton.button == sf::Mouse::Left)
	{
		switch (mStates)
		{
		case States::Play:
			mPlayState.ButtonPress();
			break;

		case States::Pause_Menu:
			ChangeState(mMenus["PauseMenu"].get()->DetectButtonPress());

			mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();

			if (mMenus["PauseMenu"].get()->GetIfForfeiting())		//Reset game when quitting out from pause menu
			{
				mConnection.CloseConnection();
				mPlayState.Reset();
			}
			break;

		case States::Enter_Name:
			ChangeState(mMenus["EnterNameMenu"].get()->DetectButtonPress());
			mPlayerName = mMenus["EnterNameMenu"].get()->GetName();		//Get the players entered name from the enter name menu
			mPlayState.SetName(mPlayerName);
			break;

		case States::Control_Menu:
			ChangeState(mMenus["ControlMenu"].get()->DetectButtonPress());
			break;

		case States::Start_Menu:
			ChangeState(mMenus["StartMenu"].get()->DetectButtonPress());
			mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();
			break;

		case States::Win_Menu:
			ChangeState(mMenus["WinMenu"].get()->DetectButtonPress());
			mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();
			break;

		case States::Disconnect_Menu:
			ChangeState(mMenus["DisconnectMenu"].get()->DetectButtonPress());
			break;

		default:
			break;
		}
		
	}
}

void Game::KeyPressed(sf::Event ev)
{
	switch (mStates)
	{
	case States::Play:
		if (ev.text.unicode == PAUSE_KEY && !mPlayState.ChatOpen() && mPlayState.IsPlayersTurn())
		{
			//Send information to server that this player has paused the game
			mStates = States::Pause_Menu;
			StartPauseTimer();
		}
		else
			mPlayState.ChatInput(ev);

		break;

	case States::Enter_Name:
		mMenus["EnterNameMenu"].get()->KeyboardInput(ev);
			break;

	default:
		break;
	}
}

void Game::ChangeState(States newState)
{
	mStates = newState;
}

#include "Game.h"

Game::Game(sf::RenderWindow& w)
	:window(w), mPlayState(w, mConnection)
{
	mMenus["StartMenu"] = std::make_unique<StartMenu>(w);
	mMenus["ControlMenu"] = std::make_unique<ControlMenu>(w);
	mMenus["LeaderboardMenu"] = std::make_unique<LeaderboardMenu>(w);
	mMenus["EnterNameMenu"] = std::make_unique<EnterNameMenu>(w);
	mMenus["PauseMenu"] = std::make_unique<PauseMenu>(w);
	mMenus["WinMenu"] = std::make_unique<WinMenu>(w);
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
	mMenus["LeaderboardMenu"].reset();
	mMenus["EnterNameMenu"].reset();
	mMenus["PauseMenu"].reset();
	mMenus["WinMenu"].reset();
	mMenus["DisconnectMenu"].reset();

}

void Game::Initialize()
{
	//Setup all elements of the game, not needed as constructors of each call initialize
	mPlayState.Initialize();

	if (!mTextFont.loadFromFile("bin/Fonts/Komika_display.ttf"))
		assert(!mTextFont.loadFromFile("bin/Fonts/Komika_display.ttf"));

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
	if (!mBackgroundTex.loadFromFile("bin/Textures/menuBackground.png"))
		assert(!mBackgroundTex.loadFromFile("bin/Textures/menuBackground.png"));

	mBackgroundTex.setSmooth(true);

	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Font
	if (!mFont.loadFromFile("bin/Fonts/Komika_display.ttf"))
		assert(!mFont.loadFromFile("bin/Fonts/Komica_display.ttf"));

	//Loading Text
	mLoadingText.setFillColor(sf::Color::White);
	mLoadingText.setFont(mFont);
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

		if (mPlayState.GetData().mDisconnected == -1)		//If disconnection has happened
		{
			mPlayState.Reset();
			mStates = States::Disconnect_Menu;
		}

		if (mPlayState.GetData().gameEnded)
		{
			mStates = States::Win_Menu;
			mMenus["WinMenu"].get()->SetWinScreenTitle(mPlayState.GetWinMessage());
			mPlayState.Reset();
		}
		break;

	case States::Matchmaking:
	{
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
		
		//Send info to server to check if matchmaking is possible (>1 clients on server)
		if (!mConnection.Matchmake())		//Couldnt find an opponent
		{
			mConnection.CloseConnection();
			mStates = States::Start_Menu;
		}
		else				//Did find an opponent
		{
			mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();
			mStates = States::Play;

			//Find from server which player this is (only returns either 1 or 2)
			int player = 0;
			
			if (!mConnection.GetPlayer(player))	
			{
				mConnection.CloseConnection();
				mStates = States::Start_Menu;
				break;
			}
			mPlayState.SetPlayer(player);

			//Send Which player will be going first, is obtained from server in PlayState::Update
			if (!mConnection.SendGameData(mPlayState.GetData()))
			{
				mConnection.CloseConnection();
				mStates = States::Start_Menu;
			}

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
		mMenus["StartMenu"].get()->Update();
		break;

	case States::Leaderboard:
		mMenus["LeaderboardMenu"].get()->Update();
		break;

	case States::Win_Menu:
		mMenus["WinMenu"].get()->Update();
		break;

	case States::Disconnect_Menu:
		mMenus["DisconnectMenu"].get()->Update();
		break;

	case States::Quit:
		window.close();
		break;

	default:
		assert(mStates);
		break;
	}

}

void Game::UpdatePauseTimer()
{
	mPauseTimer.second = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(mPauseTimer.second - mPauseTimer.first).count() / 1000000.f;

	mPauseTimerTxt.setString(std::to_string(static_cast<int>(pauseTimerAllowance - elapsedTime)));		//Shows player how long they have left on the pause menu

	if (elapsedTime > pauseTimerAllowance)
	{
		mStates = States::Play;
		mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();
		if(pauseTimerAllowance > 10)	
			pauseTimerAllowance *= 0.5f;
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
		window.draw(mPauseTimerTxt);
		break;

	case States::Control_Menu:
		mMenus["ControlMenu"].get()->Draw();
		break;

	case States::Start_Menu:
		mMenus["StartMenu"].get()->Draw();
		break;

	case States::Leaderboard:
		mMenus["LeaderboardMenu"].get()->Draw();
		break;

	case States::Win_Menu:
		mMenus["WinMenu"].get()->Draw();
		break;

	case States::Disconnect_Menu:
		mPlayState.Draw();
		mMenus["DisconnectMenu"].get()->Draw();
		break;

	default:
		assert(mStates);
		break;
	}
}

void Game::DrawMatchmakingScreen()
{
	window.draw(mBackgroundSpr);
	window.draw(mLoadingText);
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
			mPlayerName = mMenus["EnterNameMenu"].get()->GetName();
			mPlayState.SetName(mPlayerName);
			break;

		case States::Control_Menu:
			ChangeState(mMenus["ControlMenu"].get()->DetectButtonPress());
			break;

		case States::Start_Menu:
			ChangeState(mMenus["StartMenu"].get()->DetectButtonPress());
			mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();
			break;

		case States::Leaderboard:
			ChangeState(mMenus["LeaderboardMenu"].get()->DetectButtonPress());
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
		if (ev.text.unicode == PAUSE_KEY && !mPlayState.GetIfChatIsOpen() && mPlayState.IsPlayersTurn())
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


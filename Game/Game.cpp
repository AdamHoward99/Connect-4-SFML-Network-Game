#include "Game.h"

Game::Game(sf::RenderWindow& w)
	:window(w), mPlayState(w), mWin(w), mPause(w)
{
	mMenus["StartMenu"] = std::make_unique<StartMenu>(w);
	mMenus["ControlMenu"] = std::make_unique<ControlMenu>(w);
	mMenus["LeaderboardMenu"] = std::make_unique<LeaderboardMenu>(w);
	mMenus["EnterNameMenu"] = std::make_unique<EnterNameMenu>(w);

	//Setup elements of game
	Initialize();
}

Game::~Game()
{
	//Remove all pointers
	mMenus["StartMenu"].reset();
	mMenus["ControlMenu"].reset();
	mMenus["LeaderboardMenu"].reset();
	mMenus["EnterNameMenu"].reset();

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
	mLoadingText = std::vector<sf::Text>(2);

	float yOffset = 300.f;
	for (size_t i = 0; i < mLoadingText.size(); i++)
	{
		mLoadingText.at(i).setFillColor(sf::Color::White);
		mLoadingText.at(i).setFont(mFont);
		mLoadingText.at(i).setCharacterSize(25);
		mLoadingText.at(i).setPosition(320.f, yOffset);
		yOffset += 100.f;
	}

	mLoadingText.at(0).setString("Finding an Opponent...");
}

void Game::Update()
{
	//Update each element of the game based on which state is active
	switch (mStates)
	{
	case States::Play:
		mPlayState.Update();

		if (mPlayState.GetIfGameWon())
		{
			mStates = States::Win_Menu;
			mWin.SetWinScreenTitle(mPlayState.GetWinMessage());
			mPlayState.Reset();
		}
		break;

	case States::Matchmaking:
		//Connection stuff here, searches on server to find other clients, if can find another player, goes to play state
		if (mConnection.ConnectToServer())
		{
			//look for other clients on the server
			//If after certain time it cannot find another client, move back to menu state
			//If can find other client, go to play state
			mStates = States::Play;
		}
		else
		{
			mLoadingText.at(1).setString("Cannot connect to server...");
			mStates = States::Start_Menu;
		}
		//If connection to server failed, break out of this and either try again or move back to menu state
		break;

	case States::Enter_Name:
		mMenus["EnterNameMenu"].get()->Update();
			break;

	case States::Pause_Menu:
		mPause.Update();
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
		mWin.Update();
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
	//Include switch like in update for each state
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
		mPause.Draw();
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
		mWin.Draw();
		break;

	default:
		assert(mStates);
		break;
	}
}

void Game::DrawMatchmakingScreen()
{
	window.draw(mBackgroundSpr);

	for (auto t : mLoadingText)
		window.draw(t);
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
			ChangeState(mPause.DetectButtonPress());

			mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();

			if (mPause.GetIfForfeiting())		//Reset game when quitting out from pause menu
				mPlayState.Reset();
			break;

		case States::Enter_Name:
			ChangeState(mMenus["EnterNameMenu"].get()->DetectButtonPress());
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
			ChangeState(mWin.DetectButtonPress());
			mPlayState.mTurnTimer.first = std::chrono::steady_clock::now();
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
		if (ev.text.unicode == PAUSE_KEY && !mPlayState.GetIfChatIsOpen())
		{
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

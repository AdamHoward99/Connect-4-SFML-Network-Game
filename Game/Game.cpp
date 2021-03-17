#include "Game.h"

Game::Game(sf::RenderWindow& w)
	:window(w), mPlayState(w), mWin(w), mPause(w)
{
	mMenus["StartMenu"] = std::make_unique<StartMenu>(w);
	mMenus["ControlMenu"] = std::make_unique<ControlMenu>(w);
	mMenus["LeaderboardMenu"] = std::make_unique<LeaderboardMenu>(w);


	//Setup elements of game
	Initialize();
}

Game::~Game()
{
	//Remove all pointers
	mMenus["StartMenu"].reset();
	mMenus["ControlMenu"].reset();
	mMenus["LeaderboardMenu"].reset();

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
			assert(mStates);
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

	default:
		break;
	}
}

void Game::ChangeState(States newState)
{
	mStates = newState;
}
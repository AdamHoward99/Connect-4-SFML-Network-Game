#include "WinMenu.h"

WinMenu::WinMenu(sf::RenderWindow& w, NetworkConnection& mConnect)
	:mWindow(w), mServer(mConnect)
{
	mButtonAmount = 3;
	mTextAmount = 4;

	mButtons = std::vector<sf::Sprite>(mButtonAmount);
	mText = std::vector<sf::Text>(mTextAmount);

	Initialize();
}

WinMenu::~WinMenu()
{
	//Delete stuff here
}

void WinMenu::Initialize()
{
	SetupTextures();
	SetupSprites();
	SetupFonts();
	SetupText();
	SetupAudio();
}

void WinMenu::SetupSprites()
{
	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Button Sprites
	float xOffset = 30.f;
	for (int i = 0; i < mButtonAmount; i++)
	{
		mButtons.at(i).setTexture(mButtonTex);
		mButtons.at(i).setScale(sf::Vector2f(0.75f, 0.75f));
		mButtons.at(i).setPosition(xOffset, 600.f);
		xOffset += 300.f;
	}
}

void WinMenu::SetupText()
{
	float xOffset = 90.f;
	for (int i = 1; i < mTextAmount; i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(mBodyFontSize);
		mText.at(i).setPosition(xOffset, 610.f);
		xOffset += 295.f;
	}

	mText.at(0).setPosition(350.f, 200.f);
	mText.at(0).setCharacterSize(mTitleFontSize);
	mText.at(0).setFillColor(sf::Color::White);
	mText.at(0).setFont(mFont);

	mText.at(1).setString("Rematch");
	mText.at(2).setString("Leaderboard");
	mText.at(2).setPosition(370.f, 610.f);
	mText.at(3).setString("Main Menu");
}

void WinMenu::Update()
{
	mMousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

	HoverOnButton();
}

void WinMenu::Draw()
{
	mWindow.draw(mBackgroundSpr);

	for (auto b : mButtons)
		mWindow.draw(b);

	for (auto t : mText)
		mWindow.draw(t);
}

States WinMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mMousePos))
	{
		mMousePos = { 0.f, 0.f };		//Prevents multiple presses of the button
		mButtonClickSfx.second.play();

		if (mServer.CheckForRematch())
			return States::Play;		//Rematch
		else
			return States::Win_Menu;
	}

	if (mButtons.at(1).getGlobalBounds().contains(mMousePos))
	{
		mButtonClickSfx.second.play();
		return States::Leaderboard;
	}

	if (mButtons.at(2).getGlobalBounds().contains(mMousePos))
	{
		mButtonClickSfx.second.play();
		mServer.CloseConnection();
		return States::Start_Menu;
	}

	return States::Win_Menu;
}

void WinMenu::HoverOnButton()
{
	for (int i = 0; i < mButtonAmount; i++)
	{
		if (mButtons.at(i).getGlobalBounds().contains(mMousePos))
			mText.at(i+1).setFillColor(sf::Color::Black);
		else
			mText.at(i+1).setFillColor(sf::Color::White);
	}
}

void WinMenu::SetWinScreenTitle(std::string str)
{
	mText.at(0).setString(str);
}
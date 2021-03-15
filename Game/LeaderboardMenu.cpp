#include "LeaderboardMenu.h"

LeaderboardMenu::LeaderboardMenu(sf::RenderWindow& mApp)
	:window(mApp)
{
	buttonAmount = 2;
	textAmount = 3;

	mButtons = std::vector<sf::Sprite>(buttonAmount);
	mText = std::vector<sf::Text>(textAmount);

	Initialize();
}

LeaderboardMenu::~LeaderboardMenu()
{
	//Delete things here
}

void LeaderboardMenu::Initialize()
{
	SetupTextures();
	SetupSprites();
	SetupFonts();
	SetupText();
	SetupAudio();
}

void LeaderboardMenu::SetupSprites()
{
	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Button Sprite
	float xOffset = 100.f;
	for (int i = 0; i < buttonAmount; i++)
	{
		mButtons.at(i).setTexture(mButtonTex);
		mButtons.at(i).setScale(0.75f, 0.75f);
		mButtons.at(i).setPosition(xOffset, 600.f);
		xOffset += 500.f;
	}
}

void LeaderboardMenu::SetupFonts()
{
	//Font for button text
	if (!mFont.loadFromFile("bin/Fonts/OpenSans-Regular.ttf"))
		assert(!mFont.loadFromFile("bin/Fonts/OpenSans-Regular.ttf"));
}

void LeaderboardMenu::SetupText()
{
	//Title 
	mText.at(0).setString("Leaderboard");
	mText.at(0).setFont(mFont);
	mText.at(0).setPosition(350.f, 100.f);
	mText.at(0).setCharacterSize(35);

	float xOffset = 180.f;
	for (int i = 1; i < textAmount; i++)
	{
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(25);
		mText.at(i).setPosition(xOffset, 610.f);
		xOffset += 470.f;
	}

	mText.at(1).setString("Back");
	mText.at(2).setString("Main Menu");
}

void LeaderboardMenu::SetupAudio()
{
	//Buffer
	if (!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"))
		assert(!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"));

	//Sound
	mButtonClickSfx.second.setBuffer(mButtonClickSfx.first);
	mButtonClickSfx.second.setVolume(30.f);
}

void LeaderboardMenu::Update()
{
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	HoverOnButton();
}

void LeaderboardMenu::Draw()
{
	window.draw(mBackgroundSpr);

	for (auto b : mButtons)
		window.draw(b);

	for (auto t : mText)
		window.draw(t);
}

States LeaderboardMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Win_Menu;
	}
	else if (mButtons.at(1).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Start_Menu;
	}

	return States::Leaderboard;
}

void LeaderboardMenu::HoverOnButton()
{
	for (int i = 0; i < buttonAmount; i++)
		if (mButtons.at(i).getGlobalBounds().contains(mousePos))
			mText.at(i + 1).setFillColor(sf::Color::Black);
		else
			mText.at(i + 1).setFillColor(sf::Color::White);
}
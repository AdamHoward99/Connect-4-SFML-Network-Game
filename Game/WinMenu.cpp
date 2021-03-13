#include "WinMenu.h"

WinMenu::WinMenu(sf::RenderWindow& w)
	:window(w)
{
	buttonAmount = 3;
	textAmount = 4;

	mButtons = std::vector<sf::Sprite>(buttonAmount);
	mText = std::vector<sf::Text>(textAmount);

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
	for (int i = 0; i < buttonAmount; i++)
	{
		mButtons.at(i).setTexture(mButtonTex);
		mButtons.at(i).setScale(sf::Vector2f(0.75f, 0.75f));
		mButtons.at(i).setPosition(xOffset, 600.f);
		xOffset += 300.f;
	}
}

void WinMenu::SetupFonts()
{
	//Font for button text
	if (!mFont.loadFromFile("bin/Fonts/OpenSans-Regular.ttf"))
	{
		assert(!mFont.loadFromFile("bin/Fonts/OpenSans-Regular.ttf"));
	}
}

void WinMenu::SetupText()
{
	float xOffset = 90.f;
	for (int i = 1; i < textAmount; i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(25);
		mText.at(i).setPosition(xOffset, 610.f);
		xOffset += 290.f;
	}

	mText.at(0).setString("You Win");			//Have win/lose options
	mText.at(0).setPosition(350.f, 200.f);
	mText.at(0).setCharacterSize(35);
	mText.at(0).setFillColor(sf::Color::White);
	mText.at(0).setFont(mFont);

	mText.at(1).setString("Rematch");
	mText.at(2).setString("Leaderboard");
	mText.at(3).setString("Main Menu");
}

void WinMenu::SetupAudio()
{
	//Setup Audio
	//Buffer
	if (!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"))
		assert(!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"));

	//Sound
	mButtonClickSfx.second.setBuffer(mButtonClickSfx.first);
	mButtonClickSfx.second.setVolume(30.f);
}

void WinMenu::Update()
{
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	HoverOnButton();
}

void WinMenu::Draw()
{
	window.draw(mBackgroundSpr);

	for (auto b : mButtons)
		window.draw(b);

	for (auto t : mText)
		window.draw(t);
}

States WinMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Play;		//Rematch
	}

	if (mButtons.at(1).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Leaderboard;
	}

	if (mButtons.at(2).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Start_Menu;
	}

	return States::Win_Menu;
}

void WinMenu::HoverOnButton()
{
	for (int i = 0; i < buttonAmount; i++)
	{
		if (mButtons.at(i).getGlobalBounds().contains(mousePos))
			mText.at(i+1).setFillColor(sf::Color::Black);
		else
			mText.at(i+1).setFillColor(sf::Color::White);
	}
}
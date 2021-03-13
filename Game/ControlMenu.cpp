#include "ControlMenu.h"

ControlMenu::ControlMenu(sf::RenderWindow& mApp)
	:window(mApp)
{
	buttonAmount = 1;
	textAmount = 3;

	mButtons = std::vector<sf::Sprite>(buttonAmount);
	mText = std::vector<sf::Text>(textAmount);

	Initialize();
}

ControlMenu::~ControlMenu()
{
	//Delete things here
}

void ControlMenu::Initialize()
{
	SetupTextures();
	SetupSprites();
	SetupFonts();
	SetupText();
	SetupAudio();
}

void ControlMenu::SetupSprites()
{
	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Button Sprite
	mButtons.at(0).setTexture(mButtonTex);
	mButtons.at(0).setScale(sf::Vector2f(0.75f, 0.75f));
	mButtons.at(0).setPosition(300.f, 600.f);
}

void ControlMenu::SetupFonts()
{
	//Font for button text
	if (!mFont.loadFromFile("bin/Fonts/OpenSans-Regular.ttf"))
	{
		assert(!mFont.loadFromFile("OpenSans-Regular.ttf"));
	}
}

void ControlMenu::SetupText()
{
	for (int i = 0; i < textAmount; i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setFont(mFont);
	}

	mText.at(0).setString("Controls");
	mText.at(0).setPosition(sf::Vector2f(350.f, 100.f));
	mText.at(0).setCharacterSize(35);

	mText.at(1).setString("Mouse the mouse horizontally to select which column to put a piece in."
		"\nLeft Click to place the piece. Aim of the game is to get 4 in a row,"
		"\neither horizontally, vertically or diagonally.");
	mText.at(1).setCharacterSize(20);
	mText.at(1).setPosition(sf::Vector2f(150.f, 250.f));

	mText.at(2).setString("Back");
	mText.at(2).setPosition(sf::Vector2f(390.f, 610.f));
	mText.at(2).setCharacterSize(25);

}

void ControlMenu::SetupAudio()
{
	//Setup Audio
	//Buffer
	if (!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"))
		assert(!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"));

	//Sound
	mButtonClickSfx.second.setBuffer(mButtonClickSfx.first);
	mButtonClickSfx.second.setVolume(30.f);
}

void ControlMenu::Update()
{
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	HoverOnButton();
}

void ControlMenu::Draw()
{
	window.draw(mBackgroundSpr);

	for (auto b : mButtons)
		window.draw(b);

	for (auto t : mText)
		window.draw(t);
}

States ControlMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Start_Menu;
	}

	return States::Control_Menu;
}

void ControlMenu::HoverOnButton()
{
	for (int i = 0; i < buttonAmount; i++)
	{
		if (mButtons.at(i).getGlobalBounds().contains(mousePos))
			mText.at(2).setFillColor(sf::Color::Black);
		else
			mText.at(2).setFillColor(sf::Color::White);
	}
}
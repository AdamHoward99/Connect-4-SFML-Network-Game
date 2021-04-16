#include "ControlMenu.h"

ControlMenu::ControlMenu(sf::RenderWindow& mApp)
	:mWindow(mApp)
{
	//Set amount of buttons and text on menu
	mButtonAmount = 1;
	mTextAmount = 3;

	mButtons = std::vector<sf::Sprite>(mButtonAmount);
	mText = std::vector<sf::Text>(mTextAmount);

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
	mButtons.at(0).setScale(0.75f, 0.75f);
	mButtons.at(0).setPosition(350.f, 600.f);
}

void ControlMenu::SetupText()
{
	for (int i = 0; i < mTextAmount; i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setFont(mFont);
	}

	mText.at(0).setString("Controls");
	mText.at(0).setPosition(380.f, 100.f);
	mText.at(0).setCharacterSize(mTitleFontSize);

	mText.at(1).setString("Two players alternate turns dropping pieces into unfilled columns,\n"
		"When a player connects 4 of their pieces in a row, they win.\n"
		"If all columns are filled, the game ends in a tie.\n"
		"Each player gets 30 seconds to make their move, \nafter this the CPU makes a move automatically for the player.\n"
		"\nMove the mouse horizontally to select a column,\n"
		"Left click to drop a piece in the selected column.\n"
		"Press 'P' to pause the game.");

	mText.at(1).setCharacterSize(20);
	mText.at(1).setPosition(150.f, 200.f);

	mText.at(2).setString("Back");
	mText.at(2).setPosition(430.f, 610.f);
	mText.at(2).setCharacterSize(mBodyFontSize);
}

void ControlMenu::Update()
{
	mMousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
	HoverOnButton();
}

void ControlMenu::Draw()
{
	mWindow.draw(mBackgroundSpr);

	for (auto b : mButtons)
		mWindow.draw(b);

	for (auto t : mText)
		mWindow.draw(t);
}

States ControlMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mMousePos))		//Back Button
	{
		mButtonClickSfx.second.play();
		return States::Start_Menu;
	}

	return States::Control_Menu;
}

void ControlMenu::HoverOnButton()
{
	if (mButtons.at(0).getGlobalBounds().contains(mMousePos))		//Change text of Back button when hovering over
		mText.at(2).setFillColor(sf::Color::Black);

	else
		mText.at(2).setFillColor(sf::Color::White);

}
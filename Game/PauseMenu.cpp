#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& w)
	:mWindow(w)
{
	//Set amount of buttons and text on the menu
	mButtonAmount = 2;
	mTextAmount = 3;

	mButtons = std::vector<sf::Sprite>(mButtonAmount);
	mText = std::vector<sf::Text>(mTextAmount);

	Initialize();
}

PauseMenu::~PauseMenu()
{
	//Delete things here
}

void PauseMenu::Initialize()
{
	SetupTextures();
	SetupSprites();
	SetupFonts();
	SetupText();
	SetupAudio();
}

void PauseMenu::SetupTextures()
{
	//Background Texture
	mPauseTex.loadFromFile("../bin/Textures/PauseMenuUI.png");
	mPauseTex.setSmooth(true);

	//Button Texture
	mButtonTex.loadFromFile("../bin/Textures/MenuButton.png");
	mButtonTex.setSmooth(true);
}

void PauseMenu::SetupSprites()
{
	//Panel Sprite
	mPauseSpr.setTexture(mPauseTex);
	mPauseSpr.setPosition(300.f, 200.f);

	//Button Sprites
	float yOffset = 400.f;
	for (int i = 0; i < mButtonAmount; i++)
	{
		mButtons.at(i).setTexture(mButtonTex);
		mButtons.at(i).setScale(0.75f, 0.75f);
		mButtons.at(i).setPosition(350.f, yOffset);
		yOffset += 150.f;
	}
}

void PauseMenu::SetupText()
{
	//Text
	float yOffset = 410.f;
	for (int i = 0; i < mTextAmount - 1; i++)
	{
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(mBodyFontSize);
		mText.at(i).setPosition(420.f, yOffset);
		yOffset += 150.f;
	}

	mText.at(0).setString("Resume");
	mText.at(1).setString("Forfeit");

	mText.at(2).setString("Paused");
	mText.at(2).setFont(mFont);
	mText.at(2).setCharacterSize(mTitleFontSize);
	mText.at(2).setPosition(400.f, 250.f);
}

void PauseMenu::Update()
{
	mMousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
	HoverOnButton();
}

void PauseMenu::Draw() 
{
	mWindow.draw(mPauseSpr);

	for (auto b : mButtons)
		mWindow.draw(b);

	for (auto t : mText)
		mWindow.draw(t);
}

States PauseMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mMousePos))		//Resume button
	{
		mButtonClickSfx.second.play();
		return States::Play;
	}
	else if (mButtons.at(1).getGlobalBounds().contains(mMousePos))	//Forfeit button
	{
		mButtonClickSfx.second.play();
		mLeftGame = true;
		return States::Start_Menu;
	}

	return States::Pause_Menu;
}

void PauseMenu::HoverOnButton()
{
	for (int i = 0; i < mButtonAmount; i++)
		if (mButtons.at(i).getGlobalBounds().contains(mMousePos))
			mText.at(i).setFillColor(sf::Color::Black);
		else 
			mText.at(i).setFillColor(sf::Color::White);

}
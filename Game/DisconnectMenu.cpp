#include "DisconnectMenu.h"

DisconnectMenu::DisconnectMenu(sf::RenderWindow& mApp)
	:mWindow(mApp)
{
	//Set amount of buttons and text on menu
	mButtonAmount = 1;
	mTextAmount = 2;

	mButtons = std::vector<sf::Sprite>(mButtonAmount);
	mText = std::vector<sf::Text>(mTextAmount);

	Initialize();
}

DisconnectMenu::~DisconnectMenu()
{
	//Delete things here
}

void DisconnectMenu::Initialize()
{
	SetupTextures();
	SetupSprites();
	SetupFonts();
	SetupText();
	SetupAudio();
}

void DisconnectMenu::SetupTextures()
{
	//Panel Texture
	mDisconnectPanelTex.loadFromFile("../bin/Textures/PauseMenuUI.png");
	mDisconnectPanelTex.setSmooth(true);

	//Button Texture
	mButtonTex.loadFromFile("../bin/Textures/MenuButton.png");
	mButtonTex.setSmooth(true);
}

void DisconnectMenu::SetupSprites()
{
	//Panel Sprite
	mDisconnectPanelSpr.setTexture(mDisconnectPanelTex);
	mDisconnectPanelSpr.setPosition(300.f, 200.f);

	//Button Sprites
	mButtons.at(0).setTexture(mButtonTex);
	mButtons.at(0).setPosition(350.f, 500.f);
	mButtons.at(0).setScale(0.75f, 0.75f);

}

void DisconnectMenu::SetupText()
{
	//Panel Description Text
	for (int i = 0; i < mTextAmount; i++)
	{
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(mBodyFontSize);
	}

	mText.at(0).setString("Opponent has disconnected");
	mText.at(0).setPosition(320.f, 320.f);
	mText.at(0).setCharacterSize(20);

	//Panel Button Text
	mText.at(1).setString("Back to Menu");
	mText.at(1).setPosition(375.f, 510.f);

}

void DisconnectMenu::Update()
{
	mMousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
	HoverOnButton();
}

void DisconnectMenu::Draw()
{
	mWindow.draw(mDisconnectPanelSpr);

	for (auto b : mButtons)
		mWindow.draw(b);

	for (auto t : mText)
		mWindow.draw(t);

}

States DisconnectMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mMousePos))		//Back to Menu button
	{
		mButtonClickSfx.second.play();
		return States::Start_Menu;
	}

	return States::Disconnect_Menu;
}

void DisconnectMenu::HoverOnButton()
{
	if (mButtons.at(0).getGlobalBounds().contains(mMousePos))
		mText.at(1).setFillColor(sf::Color::Black);

	else
		mText.at(1).setFillColor(sf::Color::White);

}
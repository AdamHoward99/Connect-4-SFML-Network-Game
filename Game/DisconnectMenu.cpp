#include "DisconnectMenu.h"

DisconnectMenu::DisconnectMenu(sf::RenderWindow& mApp)
	:window(mApp)
{
	buttonAmount = 1;
	textAmount = 2;

	mButtons = std::vector<sf::Sprite>(buttonAmount);
	mText = std::vector<sf::Text>(textAmount);

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
	if (!mDisconnectPanelTex.loadFromFile("bin/Textures/PauseMenuUI.png"))
		assert(!mDisconnectPanelTex.loadFromFile("bin/Textures/PauseMenuUI.png"));

	mDisconnectPanelTex.setSmooth(true);

	//Button Texture
	if (!mButtonTex.loadFromFile("bin/Textures/MenuButton.png"))
		assert(!mButtonTex.loadFromFile("MenuButton.png"));
	
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
	for (int i = 0; i < textAmount; i++)
	{
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(BodyFontSize);
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
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	HoverOnButton();
}

void DisconnectMenu::Draw()
{
	window.draw(mDisconnectPanelSpr);
	
	window.draw(mButtons.at(0));

	for (auto t : mText)
		window.draw(t);

}

States DisconnectMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Start_Menu;
	}

	return States::Disconnect_Menu;
}

void DisconnectMenu::HoverOnButton()
{
	if (mButtons.at(0).getGlobalBounds().contains(mousePos))
		mText.at(1).setFillColor(sf::Color::Black);
	else
		mText.at(1).setFillColor(sf::Color::White);

}

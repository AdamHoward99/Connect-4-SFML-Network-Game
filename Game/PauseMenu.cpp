#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& w)
	:window(w)
{
	buttonAmount = 2;
	textAmount = 2;

	mButtons = std::vector<sf::Sprite>(buttonAmount);
	mText = std::vector<sf::Text>(textAmount);

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
	if (!mBackgroundTex.loadFromFile("bin/Textures/PauseMenuUI.png"))
	{
		assert(!mBackgroundTex.loadFromFile("bin/Textures/PauseMenuUI.png"));
	}
	mBackgroundTex.setSmooth(true);

	//Button Texture
	if (!mButtonTex.loadFromFile("bin/Textures/MenuButton.png"))
	{
		assert(!mButtonTex.loadFromFile("MenuButton.png"));
	}
	mButtonTex.setSmooth(true);
}

void PauseMenu::SetupSprites()
{
	//Background Sprite		//Change into a panel instead
	mBackgroundSpr.setTexture(mBackgroundTex);
	mBackgroundSpr.setPosition(350.f, 100.f);

	//Button Sprite
	mButtons.at(0).setTexture(mButtonTex);
	mButtons.at(0).setScale(0.75f, 0.75f);
	mButtons.at(0).setPosition(400.f, 200.f);

	mButtons.at(1).setTexture(mButtonTex);
	mButtons.at(1).setScale(0.75f, 0.75f);
	mButtons.at(1).setPosition(400.f, 400.f);
}

void PauseMenu::SetupText()
{
	mText.at(0).setString("Resume");
	mText.at(0).setFont(mFont);
	mText.at(0).setPosition(410.f, 220.f);
	mText.at(0).setCharacterSize(BodyFontSize);

	mText.at(1).setString("Forfeit");
	mText.at(1).setFont(mFont);
	mText.at(1).setPosition(410.f, 420.f);
	mText.at(1).setCharacterSize(BodyFontSize);
}

void PauseMenu::Update()
{
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	HoverOnButton();
}

void PauseMenu::Draw() 
{
	window.draw(mBackgroundSpr);

	for (auto b : mButtons)
		window.draw(b);

	for (auto t : mText)
		window.draw(t);
}

States PauseMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Play;
	}
	else if (mButtons.at(1).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		leftGame = true;
		return States::Start_Menu;
	}

	return States::Pause_Menu;
}

void PauseMenu::HoverOnButton()
{
	for (int i = 0; i < buttonAmount; i++)
		if (mButtons.at(i).getGlobalBounds().contains(mousePos))
			mText.at(i).setFillColor(sf::Color::Black);
		else 
			mText.at(i).setFillColor(sf::Color::White);

}
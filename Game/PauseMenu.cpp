#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& w)
	:window(w)
{
	buttonAmount = 2;
	textAmount = 3;

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
	if (!mPauseTex.loadFromFile("bin/Textures/PauseMenuUI.png"))
	{
		assert(!mPauseTex.loadFromFile("bin/Textures/PauseMenuUI.png"));
	}
	mPauseTex.setSmooth(true);

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
	mPauseSpr.setTexture(mPauseTex);
	mPauseSpr.setPosition(300.f, 200.f);

	//Button Sprites
	float yOffset = 400.f;
	for (int i = 0; i < buttonAmount; i++)
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
	for (int i = 0; i < textAmount - 1; i++)
	{
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(BodyFontSize);
		mText.at(i).setPosition(420.f, yOffset);
		yOffset += 150.f;
	}

	mText.at(0).setString("Resume");
	mText.at(1).setString("Forfeit");

	mText.at(2).setString("Paused");
	mText.at(2).setFont(mFont);
	mText.at(2).setCharacterSize(TitleFontSize);
	mText.at(2).setPosition(400.f, 250.f);
}

void PauseMenu::Update()
{
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	HoverOnButton();
}

void PauseMenu::Draw() 
{
	window.draw(mPauseSpr);

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
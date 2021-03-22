#include "EnterNameMenu.h"

EnterNameMenu::EnterNameMenu(sf::RenderWindow& mApp)
	:window(mApp)
{
	buttonAmount = 2;
	textAmount = 4;

	mButtons = std::vector<sf::Sprite>(buttonAmount);
	mText = std::vector<sf::Text>(textAmount);

	Initialize();
}

EnterNameMenu::~EnterNameMenu()
{
	//Delete things here
}

void EnterNameMenu::Initialize()
{
	SetupTextures();
	SetupSprites();
	SetupFonts();
	SetupText();
	SetupAudio();
}

void EnterNameMenu::SetupTextures()
{
	//Background Texture
	if (!mBackgroundTex.loadFromFile("bin/Textures/menuBackground.png"))
		assert(!mBackgroundTex.loadFromFile("bin/Textures/menuBackground.png"));

	mBackgroundTex.setSmooth(true);

	//Button Texture
	if (!mButtonTex.loadFromFile("bin/Textures/MenuButton.png"))
		assert(!mButtonTex.loadFromFile("bin/Textures/MenuButton.png"));

	mButtonTex.setSmooth(true);

	//Textbox Texture
	if (!mTextBoxTex.loadFromFile("bin/Textures/EnterBox.png"))
		assert(!mTextBoxTex.loadFromFile("bin/Textures/EnterBox.png"));

	mTextBoxTex.setSmooth(true);
}

void EnterNameMenu::SetupSprites()
{
	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Button Sprites
	float xOffset = 150.f;
	for (int i = 0; i < buttonAmount; i++)
	{
		mButtons.at(i).setTexture(mButtonTex);
		mButtons.at(i).setPosition(xOffset, 600.f);
		mButtons.at(i).setScale(0.75f, 0.75f);
		xOffset += 400.f;
	}

	//Textbox Sprite
	mTextboxSpr.setTexture(mTextBoxTex);
	mTextboxSpr.setPosition(320.f, 360.f);
	mTextboxSpr.setScale(0.5f, 0.15f);
}

void EnterNameMenu::SetupText()
{
	//Text for Buttons
	float xOffset = 200.f;
	for (int i = 0; i < textAmount; i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setPosition(xOffset, 610.f);
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(BodyFontSize);
		xOffset += 430.f;
	}

	//Text for title
	mText.at(2).setPosition(370.f, 150.f);
	mText.at(2).setCharacterSize(TitleFontSize);
	mText.at(2).setString("Enter Name");

	//Text for name input
	mText.at(3).setPosition(370.f, 365.f);
	mText.at(3).setFillColor(sf::Color::Black);

	mText.at(0).setString("Find Match");
	mText.at(1).setString("Back");
}

void EnterNameMenu::Update()
{
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	HoverOnButton();
}

void EnterNameMenu::Draw()
{
	window.draw(mBackgroundSpr);
	window.draw(mTextboxSpr);
	
	for (auto b : mButtons)
		window.draw(b);

	for (auto t : mText)
		window.draw(t);
}

States EnterNameMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();

		if (mNameString.getSize() > 0)		//Only procede when a name has been entered
			return States::Matchmaking;
		else
			return States::Enter_Name;
	}
	else if (mButtons.at(1).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Start_Menu;
	}
	else
		return States::Enter_Name;
}

void EnterNameMenu::HoverOnButton()
{
	for (int i = 0; i < buttonAmount; i++)
	{
		if (mButtons.at(i).getGlobalBounds().contains(mousePos))
			mText.at(i).setFillColor(sf::Color::Black);
		else
			mText.at(i).setFillColor(sf::Color::White);
	}
}

void EnterNameMenu::KeyboardInput(sf::Event ev)
{
	if (ev.text.unicode == BACKSPACE_KEY)
	{
		if (mNameString.getSize() > 0)
		{
			mNameString.erase(mNameString.getSize() - 1);
			mText.at(3).setString(mNameString);
		}
	}
	
	else if (mNameString.getSize() < mNameSize && ev.text.unicode != SPACE_KEY)
	{
		mNameString += ev.text.unicode;
		mText.at(3).setString(mNameString);
	}
}

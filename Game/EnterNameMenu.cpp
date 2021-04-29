#include "EnterNameMenu.h"

EnterNameMenu::EnterNameMenu(sf::RenderWindow& mApp)
	:mWindow(mApp)
{
	//Set amount of buttons and text on menu
	mButtonAmount = 2;
	mTextAmount = 4;

	mButtons = std::vector<sf::Sprite>(mButtonAmount);
	mText = std::vector<sf::Text>(mTextAmount);

	Initialize();
}

EnterNameMenu::~EnterNameMenu()
{}

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
	mBackgroundTex.loadFromFile("../bin/Textures/menuBackground.png");
	mBackgroundTex.setSmooth(true);

	//Button Texture
	mButtonTex.loadFromFile("../bin/Textures/MenuButton.png");
	mButtonTex.setSmooth(true);

	//Textbox Texture
	mTextBoxTex.loadFromFile("../bin/Textures/EnterBox.png");
	mTextBoxTex.setSmooth(true);
}

void EnterNameMenu::SetupSprites()
{
	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Button Sprites
	float xOffset = 150.f;
	for (int i = 0; i < mButtonAmount; i++)
	{
		mButtons.at(i).setTexture(mButtonTex);
		mButtons.at(i).setPosition(xOffset, 600.f);
		mButtons.at(i).setScale(0.75f, 0.75f);
		xOffset += 400.f;
	}

	//Textbox Sprite
	mTextBoxSpr.setTexture(mTextBoxTex);
	mTextBoxSpr.setPosition(320.f, 360.f);
	mTextBoxSpr.setScale(0.5f, 0.15f);
}

void EnterNameMenu::SetupText()
{
	//Text for Buttons
	float xOffset = 200.f;
	for (int i = 0; i < mTextAmount; i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setPosition(xOffset, 610.f);
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(mBodyFontSize);
		xOffset += 430.f;
	}

	//Text for title
	mText.at(2).setPosition(370.f, 150.f);
	mText.at(2).setCharacterSize(mTitleFontSize);
	mText.at(2).setString("Enter Name");

	//Text for name input
	mText.at(3).setPosition(370.f, 365.f);
	mText.at(3).setFillColor(sf::Color::Black);

	mText.at(0).setString("Find Match");
	mText.at(1).setString("Back");
}

void EnterNameMenu::Update()
{
	mMousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
	HoverOnButton();
}

void EnterNameMenu::Draw()
{
	mWindow.draw(mBackgroundSpr);
	mWindow.draw(mTextBoxSpr);

	for (auto b : mButtons)
		mWindow.draw(b);

	for (auto t : mText)
		mWindow.draw(t);
}

States EnterNameMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mMousePos))
	{
		mButtonClickSfx.second.play();

		if (mNameString.getSize() > 0)		//Only procede when a name has been entered
		{
			mName = mNameString;
			return States::Matchmaking;
		}

	}
	else if (mButtons.at(1).getGlobalBounds().contains(mMousePos))
	{
		mButtonClickSfx.second.play();
		return States::Start_Menu;
	}

	return States::Enter_Name;
}

void EnterNameMenu::HoverOnButton()
{
	for (int i = 0; i < mButtonAmount; i++)
	{
		if (mButtons.at(i).getGlobalBounds().contains(mMousePos))
			mText.at(i).setFillColor(sf::Color::Black);
		else
			mText.at(i).setFillColor(sf::Color::White);
	}
}

void EnterNameMenu::KeyboardInput(sf::Event ev)
{
	if (ev.text.unicode == BACKSPACE_KEY)		//Remove from the string
	{
		if (mNameString.getSize() > 0)
		{
			mNameString.erase(mNameString.getSize() - 1);
			mText.at(3).setString(mNameString);
		}
	}

	else if (mNameString.getSize() < mNameSize && ev.text.unicode != SPACE_KEY)		//Prevents spaces in player names
	{
		mNameString += ev.text.unicode;
		mText.at(3).setString(mNameString);
	}
}
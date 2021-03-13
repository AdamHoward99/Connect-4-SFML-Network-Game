#include "StartMenu.h"

StartMenu::StartMenu(sf::RenderWindow& w)
	:window(w)
{
	buttonAmount = 3;
	textAmount = 3;

	mButtons = std::vector<sf::Sprite>(buttonAmount);
	mText = std::vector<sf::Text>(textAmount);

	Initialize();
}

StartMenu::~StartMenu()
{
	//Delete stuff here
}

void StartMenu::Initialize()
{
	SetupTextures();
	SetupSprites();
	SetupFonts();
	SetupText();
	SetupAudio();
}

void StartMenu::SetupSprites()
{
	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Button Sprites
	float yOffset = 300.f;
	for (int i = 0; i < buttonAmount; i++)
	{
		mButtons.at(i).setTexture(mButtonTex);
		mButtons.at(i).setPosition(300.f, yOffset);
		mButtons.at(i).setScale(sf::Vector2f(0.75f, 0.75f));

		yOffset += 140.f;
	}
}

void StartMenu::SetupFonts()
{
	//Font for button text
	if (!mFont.loadFromFile("OpenSans-Regular.ttf"))
	{
		assert(!mFont.loadFromFile("OpenSans-Regular.ttf"));
	}
}

void StartMenu::SetupText()
{
	float yOffset = 310.f;
	for (int i = 0; i < textAmount; i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setPosition(sf::Vector2f(350.f, yOffset));
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(25);

		yOffset += 140.f;
	}

	mText.at(0).setString("Play Game");
	mText.at(1).setString("Controls");
	mText.at(2).setString("Quit Game");

	//Reposition one of the text, could remove in future
	mText.at(1).setPosition(sf::Vector2f(360.f, 450.f));

}

void StartMenu::SetupAudio()
{
	//Setup Audio
	//Buffer
	if(!mButtonClickSfx.first.loadFromFile("ButtonClickSound.wav"))
		assert(!mButtonClickSfx.first.loadFromFile("ButtonClickSound.wav"));

	//Sound
	mButtonClickSfx.second.setBuffer(mButtonClickSfx.first);
	mButtonClickSfx.second.setVolume(30.f);

	//Setup Music
	if (!mBackgroundMusic.openFromFile("BackgroundMusic.ogg"))
		assert(!mBackgroundMusic.openFromFile("BackgroundMusic.ogg"));

	mBackgroundMusic.setVolume(50.f);
	mBackgroundMusic.play();
	mBackgroundMusic.setLoop(true);
}

void StartMenu::Update()
{
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	HoverOnButton();
}

void StartMenu::Draw()
{
	window.draw(mBackgroundSpr);

	for (auto b : mButtons)
		window.draw(b);

	for (auto t : mText)
		window.draw(t);
}

States StartMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Play;
	}
	else if (mButtons.at(1).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Control_Menu;
	}
	else if (mButtons.at(2).getGlobalBounds().contains(mousePos))
	{
		mButtonClickSfx.second.play();
		return States::Quit;		//Maybe just close the window instead?
	}

	return States::Start_Menu;
}

void StartMenu::HoverOnButton()
{
	for (int i = 0; i < buttonAmount; i++)
	{
		if (mButtons.at(i).getGlobalBounds().contains(mousePos))
			mText.at(i).setFillColor(sf::Color::Black);
		else
			mText.at(i).setFillColor(sf::Color::White);
	}
}
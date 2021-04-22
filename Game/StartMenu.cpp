#include "StartMenu.h"

StartMenu::StartMenu(sf::RenderWindow& w)
	:mWindow(w)
{
	//Set amount of buttons and text on the menu
	mButtonAmount = 3;
	mTextAmount = 3;

	mButtons = std::vector<sf::Sprite>(mButtonAmount);
	mText = std::vector<sf::Text>(mTextAmount);

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

void StartMenu::SetupTextures()
{
	//Background Texture
	mBackgroundTex.loadFromFile("../bin/Textures/menuBackground.png");
	mBackgroundTex.setSmooth(true);

	//Button Texture
	mButtonTex.loadFromFile("../bin/Textures/MenuButton.png");
	mButtonTex.setSmooth(true);

	//Play Sound Texture
	mMuteButtonPlay.loadFromFile("../bin/Textures/MusicPlay.png");
	mMuteButtonPlay.setSmooth(true);

	//Mute Sound Texture
	mMuteButtonMuted.loadFromFile("../bin/Textures/MusicMute.png");
	mMuteButtonMuted.setSmooth(true);

	//Logo Texture
	mLogoTex.loadFromFile("../bin/Textures/logo.png");
	mLogoTex.setSmooth(true);
}

void StartMenu::SetupSprites()
{
	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Button Sprites
	float yOffset = 300.f;
	for (int i = 0; i < mButtonAmount; i++)
	{
		mButtons.at(i).setTexture(mButtonTex);
		mButtons.at(i).setPosition(350.f, yOffset);
		mButtons.at(i).setScale(0.75f, 0.75f);

		yOffset += 140.f;
	}

	//Mute Sprite
	mMuteButtonSpr.setTexture(mMuteButtonPlay);
	mMuteButtonSpr.setPosition(750.f, 50.f);
	mMuteButtonSpr.setScale(0.1f, 0.1f);

	//Logo Sprite
	mLogoSpr.setTexture(mLogoTex);
	mLogoSpr.setPosition(300.f, 100.f);
	mLogoSpr.setScale(0.9f, 0.9f);
}

void StartMenu::SetupText()
{
	//Button text
	float yOffset = 310.f;
	for (int i = 0; i < mTextAmount; i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setPosition(400.f, yOffset);
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(mBodyFontSize);
		yOffset += 140.f;
	}

	mText.at(0).setString("Play Game");
	mText.at(1).setString("Controls");
	mText.at(2).setString("Quit Game");

	mText.at(1).setPosition(410.f, 450.f);
}

void StartMenu::SetupAudio()
{
	//Setup Audio
	//Buffer
	mButtonClickSfx.first.loadFromFile("../bin/Music/ButtonClickSound.wav");

	//Sound
	mButtonClickSfx.second.setBuffer(mButtonClickSfx.first);
	mButtonClickSfx.second.setVolume(30.f);

	//Setup Music
	mBackgroundMusic.openFromFile("../bin/Music/BackgroundMusic.ogg");

	mBackgroundMusic.setVolume(40.f);
	mBackgroundMusic.play();
	mBackgroundMusic.setLoop(true);
}

void StartMenu::Update()
{
	mMousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
	HoverOnButton();
}

void StartMenu::Draw()
{
	mWindow.draw(mBackgroundSpr);
	mWindow.draw(mMuteButtonSpr);
	mWindow.draw(mLogoSpr);

	for (auto b : mButtons)
		mWindow.draw(b);

	for (auto t : mText)
		mWindow.draw(t);
}

States StartMenu::DetectButtonPress()
{
	if (mButtons.at(0).getGlobalBounds().contains(mMousePos))	//Play Game button
	{
		mButtonClickSfx.second.play();
		return States::Enter_Name;
	}
	else if (mButtons.at(1).getGlobalBounds().contains(mMousePos))	//Controls button
	{
		mButtonClickSfx.second.play();
		return States::Control_Menu;
	}
	else if (mButtons.at(2).getGlobalBounds().contains(mMousePos))	//Quit button
	{
		mButtonClickSfx.second.play();
		return States::Quit;
	}
	else if (mMuteButtonSpr.getGlobalBounds().contains(mMousePos))	//Mute button
	{
		if (mBackgroundMusic.getVolume() > 0)		//Mutes music only, not sfx
		{
			mBackgroundMusic.setVolume(0);
			mMuteButtonSpr.setTexture(mMuteButtonMuted);
		}
		else
		{
			mBackgroundMusic.setVolume(30);			//Unmutes music
			mMuteButtonSpr.setTexture(mMuteButtonPlay);
		}
	}

	return States::Start_Menu;
}

void StartMenu::HoverOnButton()
{
	for (int i = 0; i < mButtonAmount; i++)
	{
		if (mButtons.at(i).getGlobalBounds().contains(mMousePos))
			mText.at(i).setFillColor(sf::Color::Black);
		else
			mText.at(i).setFillColor(sf::Color::White);
	}
}
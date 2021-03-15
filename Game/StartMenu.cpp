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

void StartMenu::SetupTextures()
{
	//Background Texture
	if (!mBackgroundTex.loadFromFile("bin/Textures/menuBackground.png"))
	{
		assert(!mBackgroundTex.loadFromFile("menuBackground.png"));
	}
	mBackgroundTex.setSmooth(true);

	//Button Texture
	if (!mButtonTex.loadFromFile("bin/Textures/MenuButton.png"))
	{
		assert(!mButtonTex.loadFromFile("MenuButton.png"));
	}
	mButtonTex.setSmooth(true);

	//Move this stuff into overriden start menu function
	//Play Sound Texture
	if (!mMuteButtonPlay.loadFromFile("bin/Textures/MusicPlay.png"))
		assert(!mMuteButtonPlay.loadFromFile("MusicPlay.png"));

	mMuteButtonPlay.setSmooth(true);

	//Mute Sound Texture
	if (!mMuteButtonMuted.loadFromFile("bin/Textures/MusicMute.png"))
		assert(!mMuteButtonMuted.loadFromFile("MusicMute.png"));

	mMuteButtonMuted.setSmooth(true);
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

	//Mute Sprite
	mMuteButtonSpr.setTexture(mMuteButtonPlay);
	mMuteButtonSpr.setPosition(sf::Vector2f(750.f, 50.f));
	mMuteButtonSpr.setScale(sf::Vector2f(0.1f, 0.1f));
}

void StartMenu::SetupText()
{
	float yOffset = 310.f;
	for (int i = 0; i < textAmount; i++)
	{
		mText.at(i).setFillColor(sf::Color::White);
		mText.at(i).setPosition(sf::Vector2f(350.f, yOffset));
		mText.at(i).setFont(mFont);
		mText.at(i).setCharacterSize(BodyFontSize);

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
	if(!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"))
		assert(!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"));

	//Sound
	mButtonClickSfx.second.setBuffer(mButtonClickSfx.first);
	mButtonClickSfx.second.setVolume(30.f);

	//Setup Music
	if (!mBackgroundMusic.openFromFile("bin/Music/BackgroundMusic.ogg"))
		assert(!mBackgroundMusic.openFromFile("bin/Music/BackgroundMusic.ogg"));

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
	window.draw(mMuteButtonSpr);

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
	else if (mMuteButtonSpr.getGlobalBounds().contains(mousePos))
	{
		if (mBackgroundMusic.getVolume() > 0)
		{
			mBackgroundMusic.setVolume(0);
			mMuteButtonSpr.setTexture(mMuteButtonMuted);
		}
		else
		{
			mBackgroundMusic.setVolume(50);
			mMuteButtonSpr.setTexture(mMuteButtonPlay);
		}
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
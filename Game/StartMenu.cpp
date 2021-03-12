#include "StartMenu.h"

StartMenu::StartMenu(sf::RenderWindow& w)
	:window(w)
{
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
}

void StartMenu::SetupTextures()
{
	//Background Texture
	if (!mBackgroundTex.loadFromFile("menuBackground.png"))
	{
		assert(!mBackgroundTex.loadFromFile("menuBackground.png"));
	}
	mBackgroundTex.setSmooth(true);

	//Button Texture
	if (!mBtnTex.loadFromFile("MenuButton.png"))
	{
		assert(!mBtnTex.loadFromFile("MenuButton.png"));
	}
	mBtnTex.setSmooth(true);
}

void StartMenu::SetupSprites()
{
	//Background Sprite
	mBackgroundSpr.setTexture(mBackgroundTex);

	//Play Button Sprite
	mPlayBtnSpr.setTexture(mBtnTex);
	mPlayBtnSpr.setPosition(sf::Vector2f(300.f, 300.f));
	mPlayBtnSpr.setScale(sf::Vector2f(0.75f, 0.75f));

	//Control Button Sprite
	mControlBtnSpr.setTexture(mBtnTex);
	mControlBtnSpr.setPosition(sf::Vector2f(300.f, 440.f));
	mControlBtnSpr.setScale(sf::Vector2f(0.75f, 0.75f));

	//Quit Button Sprite
	mQuitBtnSpr.setTexture(mBtnTex);
	mQuitBtnSpr.setPosition(sf::Vector2f(300.f, 580.f));
	mQuitBtnSpr.setScale(sf::Vector2f(0.75f, 0.75f));
}

void StartMenu::SetupFonts()
{
	//Font for button text
	if (!mTxtFont.loadFromFile("OpenSans-Regular.ttf"))
	{
		assert(!mTxtFont.loadFromFile("OpenSans-Regular.ttf"));
	}
}

void StartMenu::SetupText()
{
	//Play Button text
	mPlayBtnTxt.setString("Play Game");
	mPlayBtnTxt.setFillColor(sf::Color::White);
	mPlayBtnTxt.setPosition(sf::Vector2f(350.f, 310.f));
	mPlayBtnTxt.setFont(mTxtFont);
	mPlayBtnTxt.setCharacterSize(25);

	//Control Button Text
	mControlBtnTxt.setString("Control Menu");
	mControlBtnTxt.setFillColor(sf::Color::White);
	mControlBtnTxt.setPosition(sf::Vector2f(330.f, 450.f));
	mControlBtnTxt.setFont(mTxtFont);
	mControlBtnTxt.setCharacterSize(25);

	//Quit Button Text
	mQuitBtnTxt.setString("Quit Game");
	mQuitBtnTxt.setFillColor(sf::Color::White);
	mQuitBtnTxt.setPosition(sf::Vector2f(350.f, 590.f));
	mQuitBtnTxt.setFont(mTxtFont);
	mQuitBtnTxt.setCharacterSize(25);
}

void StartMenu::Update()
{
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	HoverOnButton();
}

void StartMenu::Draw()
{
	window.draw(mBackgroundSpr);

	window.draw(mPlayBtnSpr);
	window.draw(mPlayBtnTxt);


	window.draw(mControlBtnSpr);
	window.draw(mControlBtnTxt);

	window.draw(mQuitBtnSpr);
	window.draw(mQuitBtnTxt);
}

States StartMenu::DetectButtonPress()
{
	sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	sf::FloatRect btn1Bounds = mPlayBtnSpr.getGlobalBounds();
	sf::FloatRect btn2Bounds = mControlBtnSpr.getGlobalBounds();
	sf::FloatRect btn3Bounds = mQuitBtnSpr.getGlobalBounds();

	if (btn1Bounds.contains(mouse))
	{
		return States::Play;
	}
	else if (btn2Bounds.contains(mouse))
	{
		return States::Control_Menu;
	}
	else if (btn3Bounds.contains(mouse))
	{
		return States::Quit;		//Maybe just close the window instead?
	}

	return States::Start_Menu;
}

void StartMenu::HoverOnButton()
{
	//Play Button
	if (mPlayBtnSpr.getGlobalBounds().contains(mousePos))
	{
		mPlayBtnTxt.setFillColor(sf::Color::Black);
		//Add sound here?
	}
	else mPlayBtnTxt.setFillColor(sf::Color::White);

	//Control Button
	if (mControlBtnSpr.getGlobalBounds().contains(mousePos))
	{
		mControlBtnTxt.setFillColor(sf::Color::Black);
		//Add sound here?
	}
	else mControlBtnTxt.setFillColor(sf::Color::White);

	//Quit Button
	if (mQuitBtnSpr.getGlobalBounds().contains(mousePos))
	{
		mQuitBtnTxt.setFillColor(sf::Color::Black);
		//Add sound here?
	}
	else mQuitBtnTxt.setFillColor(sf::Color::White);

}
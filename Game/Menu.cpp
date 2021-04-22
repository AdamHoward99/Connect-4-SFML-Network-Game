#include "Menu.h"

Menu::~Menu()
{
	//Delete anything here
}

void Menu::Initialize()
{

}

void Menu::Update()
{

}

void Menu::Draw()
{

}

void Menu::SetupTextures()
{
	//Background Texture
	mBackgroundTex.loadFromFile("../bin/Textures/menuBackground.png");
	mBackgroundTex.setSmooth(true);

	//Button Texture
	mButtonTex.loadFromFile("../bin/Textures/MenuButton.png");
	mButtonTex.setSmooth(true);

	//Logo Texture
	mLogoTex.loadFromFile("../bin/Textures/logo.png");
	mLogoTex.setSmooth(true);
}

void Menu::SetupFonts()
{
	//Font for button text
	if (!mFont.loadFromFile("../bin/Fonts/Komika_display.ttf"))
		exit(0);

}

void Menu::SetupAudio()
{
	//Setup Audio
	//Buffer
	mButtonClickSfx.first.loadFromFile("../bin/Music/ButtonClickSound.wav");

	//Sound
	mButtonClickSfx.second.setBuffer(mButtonClickSfx.first);
	mButtonClickSfx.second.setVolume(30.f);
}

States Menu::DetectButtonPress()
{
	return States::Start_Menu;		//Default Value
}

void Menu::KeyboardInput(sf::Event ev)
{

}

sf::String Menu::GetName()
{
	return mName;
}

bool Menu::GetIfForfeiting()
{
	return mLeftGame;
}
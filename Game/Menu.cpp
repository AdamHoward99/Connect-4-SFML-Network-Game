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
}

void Menu::SetupFonts()
{
	//Font for button text
	if (!mFont.loadFromFile("bin/Fonts/OpenSans-Regular.ttf"))
	{
		assert(!mFont.loadFromFile("bin/Fonts/OpenSans-Regular.ttf"));
	}
}

void Menu::SetupAudio()
{
	//Setup Audio
	//Buffer
	if (!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"))
		assert(!mButtonClickSfx.first.loadFromFile("bin/Music/ButtonClickSound.wav"));

	//Sound
	mButtonClickSfx.second.setBuffer(mButtonClickSfx.first);
	mButtonClickSfx.second.setVolume(30.f);
}

States Menu::DetectButtonPress()
{
	return States::Start_Menu;
}
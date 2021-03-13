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
	if (!mBackgroundTex.loadFromFile("menuBackground.png"))
	{
		assert(!mBackgroundTex.loadFromFile("menuBackground.png"));
	}
	mBackgroundTex.setSmooth(true);

	//Button Texture
	if (!mButtonTex.loadFromFile("MenuButton.png"))
	{
		assert(!mButtonTex.loadFromFile("MenuButton.png"));
	}
	mButtonTex.setSmooth(true);

	//Play Sound Texture
	if (!mMuteButtonPlay.loadFromFile("MusicPlay.png"))
		assert(!mMuteButtonPlay.loadFromFile("MusicPlay.png"));

	mMuteButtonPlay.setSmooth(true);

	//Mute Sound Texture
	if (!mMuteButtonMuted.loadFromFile("MusicMute.png"))
		assert(!mMuteButtonMuted.loadFromFile("MusicMute.png"));

	mMuteButtonMuted.setSmooth(true);
}

States Menu::DetectButtonPress()
{
	return States::Start_Menu;
}
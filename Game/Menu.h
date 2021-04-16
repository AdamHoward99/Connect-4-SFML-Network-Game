#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "States.h"
#include <cassert>

#define BACKSPACE_KEY 8
#define ENTER_KEY 13
#define SPACE_KEY 32

class Menu
{
public:
	~Menu();		//Default destructor

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual States DetectButtonPress();
	virtual void HoverOnButton() = 0;

	virtual void KeyboardInput(sf::Event ev);

	virtual sf::String GetName() { return mName; }

	virtual bool GetIfForfeiting() { return mLeftGame; }

	virtual void SetWinScreenTitle(std::string);		//Network stuff might be changing this

	//Setup functions
	virtual void SetupTextures();
	virtual void SetupSprites() = 0;
	virtual void SetupFonts();
	virtual void SetupText() = 0;
	virtual void SetupAudio();

protected:
	//Texture Variables
	sf::Texture mBackgroundTex;
	sf::Texture mButtonTex;

	sf::Texture mMuteButtonPlay;
	sf::Texture mMuteButtonMuted;

	sf::Texture mLogoTex;

	//Sprite Variables
	sf::Sprite mBackgroundSpr;
	std::vector<sf::Sprite> mButtons;
	sf::Sprite mLogoSpr;

	//Font Variables
	sf::Font mFont;

	//Text Variables
	std::vector<sf::Text> mText;

	//SFX Variables
	std::pair<sf::SoundBuffer, sf::Sound> mButtonClickSfx;

	//Music Variables
	sf::Music mBackgroundMusic;		//Jazz

	int mButtonAmount = 0;
	int mTextAmount = 0;

	sf::Vector2f mMousePos;

	const int mTitleFontSize = 35;
	const int mBodyFontSize = 25;

	const float mGameHeight = 800.f;
	const float mGameWidth = 900.f;

	sf::String mName;

	bool mLeftGame;

private:

};
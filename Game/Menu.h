#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "States.h"
#include <cassert>

class Menu
{
public:
	~Menu();		//Default destructor

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual States DetectButtonPress();
	virtual void HoverOnButton() = 0;

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

	int buttonAmount = 0;
	int textAmount = 0;

	sf::Vector2f mousePos;

	const int TitleFontSize = 35;
	const int BodyFontSize = 25;

	const float GameHeight = 800.f;
	const float GameWidth = 900.f;

private:

};
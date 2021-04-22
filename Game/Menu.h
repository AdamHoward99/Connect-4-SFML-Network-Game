#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "States.h"

//Keyboard Shortcuts
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
	virtual void HoverOnButton() = 0;
	virtual void KeyboardInput(sf::Event ev);

	//Setup functions
	virtual void SetupTextures();
	virtual void SetupSprites() = 0;
	virtual void SetupFonts();
	virtual void SetupText() = 0;
	virtual void SetupAudio();

	virtual States DetectButtonPress();
	
	//Obtains the players name from the enter name menu
	virtual sf::String GetName();
	
	//Detects when player leaves the game from the pause menu
	virtual bool GetIfForfeiting();

protected:
	//Constant App Dimensions
	const float mGameHeight = 800.f;
	const float mGameWidth = 900.f;

	//Constant Text Sizes
	const int mTitleFontSize = 35;
	const int mBodyFontSize = 25;

	//Background Variables
	sf::Texture mBackgroundTex;
	sf::Sprite mBackgroundSpr;
	
	//Button Variables
	sf::Texture mButtonTex;
	std::vector<sf::Sprite> mButtons;
	int mButtonAmount = 0;

	//Mute Button
	sf::Texture mMuteButtonPlay;
	sf::Texture mMuteButtonMuted;

	//Logo
	sf::Texture mLogoTex;
	sf::Sprite mLogoSpr;

	//Font Variables
	sf::Font mFont;

	//Text Variables
	int mTextAmount = 0;
	std::vector<sf::Text> mText;

	//SFX Variables
	std::pair<sf::SoundBuffer, sf::Sound> mButtonClickSfx;

	//Music Variables
	sf::Music mBackgroundMusic;		//Jazz Music

	//Stores mouse location
	sf::Vector2f mMousePos;

	//Name of the player
	sf::String mName;

	//Check if a player has left the game or been disconnected
	bool mLeftGame;
};
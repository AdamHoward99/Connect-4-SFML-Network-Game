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

	//Setup functions
	virtual void SetupTextures() = 0;
	virtual void SetupSprites() = 0;
	virtual void SetupFonts() = 0;
	virtual void SetupText() = 0;
	virtual void SetupAudio() = 0;


protected:
	//Texture Variables
	sf::Texture mBackgroundTex;
	sf::Texture mButtonTex;

	//Sprite Variables
	sf::Sprite mBackgroundSpr;
	std::vector<sf::Sprite> mButtons;

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

private:

};
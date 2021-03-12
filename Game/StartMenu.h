#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "States.h"
#include <cassert>

class StartMenu
{
public:
	StartMenu(sf::RenderWindow& mApp);
	~StartMenu();

	void Initialize();
	void Update();
	void Draw();

	States DetectButtonPress();

private:
	sf::RenderWindow& window;

	void SetupTextures();
	void SetupSprites();
	void SetupFonts();
	void SetupText();

	void HoverOnButton();

	//Texture Variables
	sf::Texture mBackgroundTex;
	sf::Texture mBtnTex;

	//Sprite Variables
	sf::Sprite mBackgroundSpr;
	sf::Sprite mPlayBtnSpr;
	sf::Sprite mControlBtnSpr;		//Could create a vector for buttons and text?
	sf::Sprite mQuitBtnSpr;

	//Font Variables
	sf::Font mTxtFont;

	//Text Variables
	sf::Text mPlayBtnTxt;
	sf::Text mControlBtnTxt;
	sf::Text mQuitBtnTxt;

	//Sound Variables
	sf::SoundBuffer mButtonSFXBuffer;
	sf::Sound mButtonSFX;

	sf::Music mBackgroundMusic;

	sf::Vector2f mousePos;
};
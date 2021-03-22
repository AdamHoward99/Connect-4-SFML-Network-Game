#pragma once

#define BACKSPACE_KEY 8
#define SPACE_KEY 32

#include "Menu.h"
#include <cassert>

class EnterNameMenu : public Menu
{
public:
	EnterNameMenu(sf::RenderWindow& mApp);		//Default Constructor
	~EnterNameMenu();							//Default Destructor

	void Initialize() override;
	void Update() override;
	void Draw() override;

	States DetectButtonPress() override;

private:
	sf::RenderWindow& window;

	void SetupTextures() override;
	void SetupSprites() override;
	void SetupText() override;

	void HoverOnButton() override;

	void KeyboardInput(sf::Event ev) override;

	sf::Texture mTextBoxTex;
	sf::Sprite mTextboxSpr;

	sf::String mNameString;
	const unsigned int mNameSize = 15;
};
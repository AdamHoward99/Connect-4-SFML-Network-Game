#pragma once
#include "Menu.h"

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
	sf::RenderWindow& mWindow;

	void SetupTextures() override;
	void SetupSprites() override;
	void SetupText() override;
	void HoverOnButton() override;
	void KeyboardInput(sf::Event ev) override;

	//Enter Name TextBox
	sf::Texture mTextBoxTex;
	sf::Sprite mTextBoxSpr;

	sf::String mNameString;
	const unsigned int mNameSize = 6;
};
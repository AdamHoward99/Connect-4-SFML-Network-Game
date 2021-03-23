#pragma once
#include "Menu.h"
#include <cassert>

class PauseMenu : public Menu
{
public:
	PauseMenu(sf::RenderWindow& mApp);		//Default Constructor
	~PauseMenu();			//Default Destructor

	void Initialize() override;
	void Update() override;
	void Draw() override;

	States DetectButtonPress() override;

private:
	sf::RenderWindow& window;

	sf::Texture mPauseTex;
	sf::Sprite mPauseSpr;

	void SetupTextures() override;
	void SetupSprites() override;
	void SetupText() override;

	void HoverOnButton() override;
};

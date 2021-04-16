#pragma once
#include "Menu.h"

class StartMenu : public Menu
{
public:
	StartMenu(sf::RenderWindow& mApp);
	~StartMenu();

	void Initialize() override;
	void Update() override;
	void Draw() override;

	States DetectButtonPress() override;

private:
	sf::RenderWindow& mWindow;

	void SetupTextures() override;
	void SetupSprites() override;
	void SetupText() override;
	void SetupAudio() override;
	void HoverOnButton() override;

	sf::Sprite mMuteButtonSpr;
};
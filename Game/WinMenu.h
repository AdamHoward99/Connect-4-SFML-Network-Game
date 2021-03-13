#pragma once
#include "Menu.h"
#include <cassert>

class WinMenu : public Menu
{
public:
	WinMenu(sf::RenderWindow& mApp);		//Default Constructor
	~WinMenu();		//Default destructor

	void Initialize() override;
	void Update() override;
	void Draw() override;

	States DetectButtonPress() override;

private:
	sf::RenderWindow& window;

	void SetupSprites() override;
	void SetupFonts() override;
	void SetupText() override;
	void SetupAudio() override;

	void HoverOnButton() override;
};
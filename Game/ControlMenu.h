#pragma once
#include "Menu.h"
#include <cassert>

class ControlMenu : public Menu
{
public:
	ControlMenu(sf::RenderWindow& mApp);		//Default Constructor
	~ControlMenu();		//Default Destructor

	void Initialize() override;
	void Update() override;
	void Draw() override;

	States DetectButtonPress() override;


private:
	sf::RenderWindow& window;

	void SetupTextures() override;
	void SetupSprites() override;
	void SetupFonts() override;
	void SetupText() override;
	void SetupAudio() override;

	void HoverOnButton() override;
};
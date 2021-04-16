#pragma once
#include "Menu.h"

class ControlMenu : public Menu
{
public:
	ControlMenu(sf::RenderWindow& mApp);		//Default Constructor
	~ControlMenu();								//Default Destructor

	void Initialize() override;
	void Update() override;
	void Draw() override;

	States DetectButtonPress() override;

private:
	sf::RenderWindow& mWindow;

	void SetupSprites() override;
	void SetupText() override;
	void HoverOnButton() override;
};
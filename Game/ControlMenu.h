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

	void SetupSprites() override;
	void SetupText() override;
	void HoverOnButton() override;
};
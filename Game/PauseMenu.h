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

	bool GetIfForfeiting() { return leftGame; }
private:
	sf::RenderWindow& window;

	void SetupSprites() override;
	void SetupText() override;

	void HoverOnButton() override;

	bool leftGame = false;
	//Incorporate a timer when doing p v p

};

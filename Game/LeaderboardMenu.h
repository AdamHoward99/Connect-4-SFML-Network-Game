#pragma once
#include "Menu.h"

class LeaderboardMenu : public Menu
{
public:
	LeaderboardMenu(sf::RenderWindow& mApp);		//Default Constructor
	~LeaderboardMenu();		//Default Destructor

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
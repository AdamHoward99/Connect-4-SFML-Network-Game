#pragma once
#include "Menu.h"
#include <cassert>

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
	sf::RenderWindow& window;

	void SetupSprites() override;
	void SetupFonts() override;
	void SetupText() override;
	void SetupAudio() override;

	void HoverOnButton() override;

};
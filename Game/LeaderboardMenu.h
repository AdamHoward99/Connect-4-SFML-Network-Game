#pragma once
#include "Menu.h"
#include "NetworkConnection.h"
#include <cassert>

class LeaderboardMenu : public Menu
{
public:
	LeaderboardMenu(sf::RenderWindow& mApp, NetworkConnection& mConnection);		//Default Constructor
	~LeaderboardMenu();		//Default Destructor

	void Initialize() override;
	void Update() override;
	void Draw() override;

	States DetectButtonPress() override;
private:
	sf::RenderWindow& window;
	NetworkConnection& mServer;

	void SetupSprites() override;
	void SetupText() override;

	void HoverOnButton() override;

};
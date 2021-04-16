#pragma once
#include "Menu.h"
#include "NetworkConnection.h"

class WinMenu : public Menu
{
public:
	WinMenu(sf::RenderWindow& mApp, NetworkConnection& mConnection);		//Default Constructor
	~WinMenu();		//Default destructor

	void Initialize() override;
	void Update() override;
	void Draw() override;

	States DetectButtonPress() override;

	void SetWinScreenTitle(std::string) override;
private:
	sf::RenderWindow& window;
	NetworkConnection& mServer;

	void SetupSprites() override;
	void SetupText() override;

	void HoverOnButton() override;
};
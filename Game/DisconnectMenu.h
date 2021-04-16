#pragma once
#include "Menu.h"

class DisconnectMenu : public Menu
{
public:
	DisconnectMenu(sf::RenderWindow& mApp);		//Default constructor
	~DisconnectMenu();							//Default destructor

	void Initialize() override;
	void Update() override;
	void Draw() override;

	States DetectButtonPress() override;

private:
	sf::RenderWindow& mWindow;

	void SetupTextures() override;
	void SetupSprites() override;
	void SetupText() override;
	void HoverOnButton() override;

	//Disconnect Notification Panel
	sf::Texture mDisconnectPanelTex;
	sf::Sprite mDisconnectPanelSpr;
};

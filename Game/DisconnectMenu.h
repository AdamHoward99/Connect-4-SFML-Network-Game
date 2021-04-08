#pragma once
#include "Menu.h"
#include <cassert>

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
	sf::RenderWindow& window;

	sf::Texture mDisconnectPanelTex;
	sf::Sprite mDisconnectPanelSpr;

	void SetupTextures() override;
	void SetupSprites() override;
	void SetupText() override;
	void HoverOnButton() override;
};

#pragma once

#include "SFML//Graphics.hpp"
#include <cassert>

class Menu
{
public:
	Menu(sf::RenderWindow& mApp);		//Default constructor
	~Menu();		//Default destructor

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

private:
	sf::RenderWindow& window;

	sf::Texture mBackgroundTex;
	sf::Sprite mBackgroundSpr;

	//Text variable for title of menu
};
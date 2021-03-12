#include "Menu.h"

Menu::Menu(sf::RenderWindow& w)
	:window(w)
{
	Initialize();
}

Menu::~Menu()
{
	//Delete anything here
}

void Menu::Initialize()
{
	if (!mBackgroundTex.loadFromFile("menuBackground.png"))
	{
		assert(!mBackgroundTex.loadFromFile("menuBackground.png"));
	}
	mBackgroundTex.setSmooth(true);

	mBackgroundSpr.setTexture(mBackgroundTex);
}

void Menu::Update()
{

}

void Menu::Draw()
{
	window.draw(mBackgroundSpr);
}
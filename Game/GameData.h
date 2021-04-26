#pragma once
#include <string>
#include "Turn.h"

#define GAMEDATA_SIZE 60

struct GameData			//Gets information for the game from the server in a single receive function, serialized when sent to server
{
	int mDisconnected = 1;				//1 = not disconnected, -1 = disconnected
	bool gameEnded = false;				//1 Represents true, 0 represents false
	Turn mTurn;
	std::pair<int, int> mLastMove;		//Stores x,y values of last piece placed from either player
	std::string mMessage = "";			//Contains message entered in chat
	std::string mWinMessage = "";		//Displays result of the game on the win screen
};
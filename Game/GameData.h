#pragma once

#include <string>
#include "Turn.h"

#define GAMEDATA_SIZE 60

struct GameData			//Gets information for the game from the server in a single receive function
{
	//turn, board update, chat
	int mDisconnected = 1;		//True when the client disconnects, 1 = not disconnected, -1 = disconnected
	bool gameEnded = false;		//1 Represents true, 0 represents false
	Turn mTurn;
	std::pair<int, int> mLastMove;
	std::string mMessage = "";		//Chat Variable
	std::string mWinMessage = "";		//Displays result of the game on the win screen
};

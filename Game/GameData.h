#pragma once

#include <string>
#include "Turn.h"

struct GameData			//Gets information for the game from the server in a single receive function
{
	//turn, board update, chat
	int mDisconnected = 0;		//True when the client disconnects, 0 =  not disconnected, 1 = disconnected
	Turn mTurn;
	std::pair<int, int> mLastMove;
	std::string mMessage = "";		//Chat Variable

};

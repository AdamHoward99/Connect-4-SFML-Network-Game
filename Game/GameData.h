#pragma once

#include <string>
#include "Turn.h"

struct GameData			//Gets information for the game from the server in a single receive function
{
	//turn, board update, chat
	bool mDisconnected = false;		//True when the client disconnects
	Turn mTurn;
	std::pair<int, int> mLastMove;
	std::string mMessage = "";		//Chat Variable

};

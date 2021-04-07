#pragma once

#include <string>
#include "Turn.h"

struct GameData			//Gets information for the game from the server in a single receive function
{
	//turn, board update, chat
	Turn mTurn;
	std::pair<int, int> mLastMove;
	std::string mMessage = "";		//Chat Variable
	//std::pair<int, int> mLastMove;

};

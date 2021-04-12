#pragma once
enum PACKET
{
	Empty,				//Has to make 0 none as if no packet is sent, 0 is default
	mMatchmakingCheck,
	mUsername,
	mPlayerType,
	mData,
	mRematch,
};
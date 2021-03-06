#pragma once
//Prevents Winsock 1.1 from being included
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#pragma comment(lib, "ws2_32.lib")

#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstdlib>
#include <chrono>

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "../Server/PACKET.h"
#include "GameData.h"

class NetworkConnection
{
public:
	NetworkConnection();		//Default constructor
	~NetworkConnection();		//Default destructor

	//Initial Setup Functions
	bool ConnectToServer();
	void CreateSocket();
	void CloseConnection();

	bool SendPlayerName(std::string username);

	bool Matchmake();		//Returns true if opponent is found
	bool GetPlayer(int& playerType);		//Returns true if type is 1 or 2
	bool GetDataUpdate();		//Checks with server for the current situation of the game

	bool CheckForRematch();

	//Match up Functions (bool value determining if other clients are on the server)
	bool GetMatch(bool &foo);
	bool SendMatch(const bool& value);

	//Getting Player Type Functions (int value returning 1 or 2)
	bool GetPlayerType(int& value);
	bool SendPlayerType(const int& value);

	//String Data Functions
	bool SendString(const std::string& message);
	bool GetString(std::string& message);

	//Int Data Functions
	bool GetInt(int& value);
	bool SendInt(const int& value) const;

	//Game Data Functions
	bool GetGameData(GameData& value);
	bool SendGameData(GameData& value);

	//Rematch Functions
	bool SendRematch(const int value);
	bool GetRematch(int& value);

	//Serialize and Deserialize Struct Functions
	void SerializeStruct(GameData* mDataPacket, char *data);
	void DeserializeStruct(GameData* mDataPacket, char *data);

	//Packet Data Functions
	bool SendPacketType(const PACKET& mType);

	//Data Verification Functions
	void VerifyData(GameData& mServerData);

	GameData mGameData;

private:
	WSAData wsaData;
	SOCKET connectSocket;
	struct sockaddr_in server;
};
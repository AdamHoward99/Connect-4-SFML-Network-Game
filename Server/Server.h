//Prevents Winsock 1.1. from being included
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define	GAMEDATA_SIZE 50
#endif

#include <Windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "PACKET.h"
#include "../Game/GameData.h"

#include <vector>
#include <thread>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>

class Server
{
public:
	Server(int port);			//Default Constructor
	~Server();					//Default Destructor

	void ListenForNewConnections();
	void CloseConnection(int index);

	static void ClientHandler(int index);

	bool ProcessPacket(int id, PACKET packetType);

	bool GetPacketType(int id, PACKET& packetType);
	bool SendPacketType(int id, const PACKET& mPacket);

	bool SendInt(int id, int value);
	bool GetInt(int id, int& value);

	bool SendString(int id, const std::string& message);
	bool GetString(int id, std::string& message);

	//Matchmaking Functions
	bool SendMatch(int id, bool value);
	bool GetMatch(int id, bool& value);

	//Player Type Function
	bool SendPlayerType(int id, int value);

	//GameData Functions
	bool SendGameData(int id, GameData value);
	bool GetGameData(int id, GameData& value);

	//Rematch Function
	bool SendRematch(int id, int value);

	//Serialize and Deserialize Functions
	void SerializeStruct(GameData* mData, char *data);
	void DeserializeStruct(GameData* mData, char *data);

	//Opponent Disconnect Functions
	bool MatchupExists(int id);
	void DeleteMatchup(int id);

	//bool functions, replaced by match functions above

	void GetUsername(int id);

	std::vector<std::string> usernames = {};

private:
	std::vector<SOCKET> mClientConnections;
	std::vector<std::thread> mConnectionThreads;
	int mConnections = 0;

	std::vector<std::pair<int, int>> mMatchups;
	std::vector<bool> mThreadActive;

	std::pair<int, int> rematchAccepted = { 0,0 };

	SOCKET listen_socket;
	addrinfo* info;
	addrinfo hints;
	int result;
};

static Server* serverPtr;
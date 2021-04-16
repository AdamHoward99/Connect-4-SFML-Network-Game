//Prevents Winsock 1.1. from being included
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define	GAMEDATA_SIZE 60
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
	bool SendGameData(int id, GameData* value);
	bool GetGameData(int id, GameData& value);

	//Rematch Function
	bool SendRematch(int id, int value);

	//Serialize and Deserialize Functions
	void SerializeStruct(GameData* mData, char *data);
	void DeserializeStruct(GameData* mData, char *data);

	//Opponent Disconnect Functions
	bool MatchupExists(int id);
	void DeleteMatchup(int id);

	void GetUsername(int id);

	std::vector<std::string> usernames = {};		//Stores all usernames of clients connecting to the server

private:
	std::vector<SOCKET> mClientConnections;
	std::vector<std::thread> mConnectionThreads;
	int mConnections = 0;

	std::vector<std::pair<int, int>> mMatchups;		//Stores indexes for client matches, deletes when one of the clients disconnects
	std::vector<bool> mThreadActive;				//Easier way to detect a disconnection without removing the thread, misaligning the vectors

	std::vector<std::pair<int, int>> mRematchAccepted;		//Stores values which change to 1 when that client is requesting a rematch, aligns with matchup vector

	SOCKET listen_socket;
	addrinfo* info;
	addrinfo hints;
	int result;
};

static Server* serverPtr;